#include <iostream>
#include "parser.h"
#include "ppm.h"
#include <cmath> // YENİ EKLEDİM

typedef unsigned char RGB[3];
using namespace parser; // YENİ EKLEDİM

double det(double a, double b, double c, double d, double e, double f, double g, double h, double i){  // 3x3 DETERMINANT FONKSIYONU
    double det = a * ( e*i - h*f ) + b * ( g*f - d*i ) + c * ( d*h - e*g );
    return det;
    }

/////////////////////VECTOR FUNCTIONS/////////////////////

Vec3f add(const Vec3f &vec1, const Vec3f &vec2){
    return {vec1.x+vec2.x, vec1.y+vec2.y, vec1.z+vec2.z};
}

Vec3f subtract(const Vec3f &vec1, const Vec3f &vec2){
    return {vec1.x-vec2.x, vec1.y-vec2.y, vec1.z-vec2.z};
}

Vec3f multiply(const Vec3f &vec1, const Vec3f &vec2){
    return {vec1.x*vec2.x, vec1.y*vec2.y, vec1.z*vec2.z};
}

Vec3f scalarMultiply(const Vec3f &vec, const double &val){
    return {vec.x*val, vec.y*val, vec.z*val};
}

Vec3f divide(const Vec3f &vec1, const Vec3f &vec2){
    return {vec1.x/vec2.x, vec1.y/vec2.y, vec1.z/vec2.z};
}

double vectorLength(const Vec3f &vec){
    return std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

double dotProduct(const Vec3f &vec1, const Vec3f & vec2){
    return ((vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z));
}

Vec3f crossProduct(const Vec3f &vec1, const Vec3f & vec2){
    return {(vec1.y*vec2.z) - (vec1.z*vec2.y), 
            (vec1.z*vec2.x) - (vec1.x*vec2.z),
            (vec1.x*vec2.y) - (vec1.y*vec2.x)};
}

Vec3f normalizeVector(const Vec3f &vec){
    double length = vectorLength(vec);

    if(length == 0.0)
        return {0,0,0};

    return {vec.x/length, vec.y/length, vec.z/length};
}
//////////////////////////////////////////////////////////



///////////////////////GENERATE RAY///////////////////////

Ray generateRay(int i, int j, const Camera &camera){

    Ray ray;
    Vec3f u, v, gaze;

    gaze = normalizeVector(camera.gaze);
    u = normalizeVector(crossProduct(gaze, camera.up));
    v = crossProduct(u, gaze);

    double su = (j + 0.5)*(camera.near_plane.y - camera.near_plane.x)/camera.image_width;
    double sv = (i + 0.5)*(camera.near_plane.w - camera.near_plane.z)/camera.image_height;


    Vec3f m, q, s;

    m = add(camera.position, scalarMultiply(gaze, camera.near_distance));

    q = add(add(m, scalarMultiply(u, camera.near_plane.x)), (scalarMultiply(v, camera.near_plane.w)));

    s = subtract(add(q, scalarMultiply(u, su)), scalarMultiply(v, sv));
    
    ray.origin = camera.position;
    ray.direction = normalizeVector(subtract(s, camera.position));
    ray.isShadow = false;

    return ray;
}


//////////////////////////////////////////////////////////



/////////////////////SHADER FUNCTIONS/////////////////////

Vec3f calculateIrradiance(const PointLight &pointLight, const Vec3f &intersection){

    Vec3f distance = subtract(pointLight.position, intersection);
    
    double dp = dotProduct(distance, distance);

    if(dp == 0.0){
        return{0,0,0};
    }

    return {pointLight.intensity.x/dp,
            pointLight.intensity.y/dp,
            pointLight.intensity.z/dp};

}

Vec3f calculateDiffuse(const PointLight &pointLight, const Scene &scene, const Vec3f &normal, const Vec3f &intersection, int material_id)
{
    Vec3f irradiance = calculateIrradiance(pointLight, intersection);

    double dotPrdct = dotProduct(normalizeVector(subtract(pointLight.position, intersection)), normal);

    if(dotPrdct < 0)
    {
        return {0,0,0};
    }

    return (scalarMultiply(multiply(scene.materials[material_id - 1].diffuse, irradiance), dotPrdct));
}



Vec3f calculateSpecular(const PointLight &pointLight, const Scene &scene, const Ray &ray, const Vec3f &normal, const Vec3f &intersection, int material_id){

    Vec3f v = normalizeVector(subtract(pointLight.position, intersection)); 
    Vec3f halfVector = normalizeVector(subtract(v, ray.direction));


    double dotPro = dotProduct(normal, halfVector);
    if(dotPro < 0)
    {
        return {0,0,0};
    }

    Vec3f irradiance = calculateIrradiance(pointLight, intersection);

    return (scalarMultiply(multiply(scene.materials[material_id - 1].specular, irradiance), pow(dotPro, scene.materials[material_id - 1].phong_exponent)));
}


Vec3f calculateRadiance(const PointLight &pointLight, const Scene &scene, const Ray &ray, pointInfo point_info, int material_id, bool isShadow, Vec3f prevRadiance){
    
    Vec3f radiance;

    if(!isShadow){
        int material_id = point_info.material_id;

        Vec3f diffuse = calculateDiffuse(pointLight, scene, point_info.normalVector, point_info.pointCoordinates, material_id);
        Vec3f specular = calculateSpecular(pointLight, scene, ray, point_info.normalVector, point_info.pointCoordinates, material_id);

        radiance = add(prevRadiance, add(diffuse, specular));

    }

    return radiance;
}


//////////////////////////////////////////////////////////


////////////////INTERSECTION FUNCTIONS///////////////////
//EĞER SHADING'I HESAPLARKEN OBJECT TYPE GEREKLIYSE  MYPOINT STRUCT'UNA EKLENEBILIR ( TRIANGLE,MESH,SPHERE )

Vec3f hitPoint(const Ray &ray, double t)
{
    Vec3f hitPointCoord;
    Vec3f d_times_t;
    d_times_t = scalarMultiply(ray.direction,t);
    hitPointCoord = add(ray.origin,d_times_t);

    return hitPointCoord;
}
///////PARAMETRE OLARAK SPHERE ID'YI DE VERDIM , PIXEL COLOR BULUNURKEN KULLANMAK İÇİN BELKİ KULLANILABİLİR
///////(POİNTINFO STRUCT'UNA DA EKLEMEK LAZIM KULLANILACAKSA, ŞU ANDA BİR İŞLEVİ YOK)

pointInfo intersectSphere(const Ray &ray, int material_id, int sphere_id, const Vec3f &center, double radius)
{

    pointInfo mypoint;

    double A,B,C;
    
    double delta;
        
    double t,t1,t2;
    
    C = dotProduct(subtract(ray.origin, center),subtract(ray.origin, center)) - (radius*radius);

    B = 2 * dotProduct(ray.direction, subtract(ray.origin, center));
    
    A = dotProduct(ray.direction, ray.direction);
    
    delta = B*B-4*A*C;
    
    if (delta<0)
        {
            mypoint.intersectHappen = false;
        }
    else if (delta==0)
    {
        t = -B / (2*A);
        mypoint.intersectHappen = true;
        mypoint.t = t;
        mypoint.material_id = material_id;
        mypoint.obj_type = 1;
        mypoint.pointCoordinates = hitPoint(ray,t);
        mypoint.normalVector = normalizeVector(subtract(mypoint.pointCoordinates,center));
    }
    else
    {
        delta = sqrt(delta);
        A = 2*A;
        t1 = (-B + delta) / A;
        t2 = (-B - delta) / A;
                
        if (t1<t2) t=t1; else t=t2;
        mypoint.intersectHappen = true;
        mypoint.t = t;
        mypoint.material_id = material_id;
        mypoint.obj_type = 1;
        mypoint.pointCoordinates = hitPoint(ray,t);
        mypoint.normalVector = normalizeVector(subtract(mypoint.pointCoordinates,center));        
    }
    
    return mypoint;
}


pointInfo intersectTriangle(const Ray &ray, int material_id, int triangle_id, const Vec3f &v0 ,const Vec3f &v1, const Vec3f &v2)
{

    pointInfo mypoint;

    double a = v0.x - v1.x,  b = v0.y - v1.y,  c = v0.z - v1.z,d = v0.x - v2.x,  e = v0.y - v2.y,  f = v0.z - v2.z,
          g = ray.direction.x, h = ray.direction.y, i = ray.direction.z, j = v0.x - ray.origin.x, k = v0.y - ray.origin.y, 
          l = v0.z - ray.origin.z;



    double A = det(a, b, c, d, e, f, g, h, i);
    double beta = det(j, k, l, d, e, f, g, h, i)/A;
    double gamma = det(a, b, c, j, k, l, g, h, i)/A;
    double t = det(a, b, c, d, e, f, j, k, l)/A;


    if(beta + gamma <= 1 && beta >= 0 && gamma >= 0)
        { 
            mypoint.pointCoordinates = hitPoint(ray,t);
            mypoint.t = t;
            mypoint.intersectHappen = true;
            mypoint.normalVector = normalizeVector(crossProduct(subtract(v0,v1), subtract(v0,v2)));
            mypoint.material_id = material_id;
            mypoint.obj_type = 2;
            mypoint.obj_id = triangle_id;
        }
    else
        {

            mypoint.intersectHappen = false;
        }


    return mypoint;


}




pointInfo intersectMesh(const Ray &ray, int material_id, const Scene &scene, int mesh_id, const Mesh &mesh)
{
    
    pointInfo mypoint;
    int faceNum = mesh.faces.size();
    Vec3f triangle_v0;
    Vec3f triangle_v1;
    Vec3f triangle_v2;
    double t;
    int vectorSize = 0;

    std::vector<pointInfo> pointInfoVector;



    for(int faceIndex = 0; faceIndex < faceNum; faceIndex++)
        {
             triangle_v0 = scene.vertex_data[mesh.faces[faceIndex].v0_id - 1];     
             triangle_v1 = scene.vertex_data[mesh.faces[faceIndex].v1_id - 1];
             triangle_v2 = scene.vertex_data[mesh.faces[faceIndex].v2_id - 1];
             mypoint = intersectTriangle(ray, material_id, mesh_id, triangle_v0, triangle_v1, triangle_v2);
             if(mypoint.intersectHappen && mypoint.t >= 0)
                {
                    mypoint.material_id = material_id;
                    mypoint.obj_type = 3;
                    mypoint.obj_id = mesh_id;
                    mypoint.pointCoordinates = hitPoint(ray, mypoint.t);
                    mypoint.normalVector = crossProduct(subtract(triangle_v1, triangle_v0), subtract(triangle_v2, triangle_v0));
                    mypoint.normalVector = normalizeVector(mypoint.normalVector);

                    pointInfoVector.push_back(mypoint);
                    vectorSize++;

                }             

        }


    pointInfo tempPoint;
    tempPoint.intersectHappen = false;

    if(vectorSize != 0)
    {
        tempPoint = pointInfoVector[0];

        for(int i = 1; i < vectorSize; i++)
        {
            if(pointInfoVector[i].t < tempPoint.t)
            {
                tempPoint = pointInfoVector[i];
            }
        }
        tempPoint.intersectHappen = true;
    }

    mypoint = tempPoint;

    return mypoint;

}




pointInfo intersectionResult(const Ray &ray, const Scene& scene)
{
    Mesh curMesh;
    Triangle curTriangle;
    Sphere curSphere;
    double t;
    Vec3f triangle_v0;
    Vec3f triangle_v1;
    Vec3f triangle_v2;
    Vec3f centerCoord;
    int vectorSize = 0;

    std::vector<pointInfo> pointInfoVector;


    for(int triangleIndex = 0; triangleIndex < scene.triangles.size(); triangleIndex++)
        {
            curTriangle = scene.triangles[triangleIndex];
            triangle_v0 = scene.vertex_data[curTriangle.indices.v0_id-1];
            triangle_v1 = scene.vertex_data[curTriangle.indices.v1_id-1];
            triangle_v2 = scene.vertex_data[curTriangle.indices.v2_id-1];
            pointInfo tempPoint = intersectTriangle(ray,curTriangle.material_id,triangleIndex,triangle_v0,triangle_v1,triangle_v2);
            if(tempPoint.intersectHappen && tempPoint.t >= 0)
                {
                    pointInfoVector.push_back(tempPoint);
                    vectorSize++;
                }



        }

    for(int sphereIndex = 0; sphereIndex< scene.spheres.size(); sphereIndex++)
        {
            curSphere = scene.spheres[sphereIndex];
            double radius = curSphere.radius;
            centerCoord = scene.vertex_data[curSphere.center_vertex_id-1];
            pointInfo tempPoint = intersectSphere(ray,curSphere.material_id,sphereIndex,centerCoord,radius);
            if(tempPoint.intersectHappen && tempPoint.t >= 0)
                {
                    pointInfoVector.push_back(tempPoint);
                    vectorSize++;
                }

        }

    for(int meshIndex = 0; meshIndex < scene.meshes.size(); meshIndex++)
        {
            curMesh = scene.meshes[meshIndex];
            pointInfo tempPoint = intersectMesh(ray,curMesh.material_id,scene,meshIndex,curMesh);
            if(tempPoint.intersectHappen && tempPoint.t >= 0)
                {
                    pointInfoVector.push_back(tempPoint);
                    vectorSize++;
                }
        }


    pointInfo mypoint;
    mypoint.intersectHappen = false;

    if(vectorSize != 0)
    {
        mypoint = pointInfoVector[0];

        for(int i = 1; i < vectorSize; i++)
        {
            if(pointInfoVector[i].t < mypoint.t)
            {
                mypoint = pointInfoVector[i];
            }
        }
        mypoint.intersectHappen = true;
    }

    return mypoint;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
bool isShadow(const Scene &scene, const pointInfo &point_info, const PointLight &currentLight){

    Vec3f nVec = normalizeVector(subtract(currentLight.position, point_info.pointCoordinates));
    Vec3f nVecEps = scalarMultiply(nVec, scene.shadow_ray_epsilon);
    bool issShadow = false;

    Ray sRay ={ add(point_info.pointCoordinates, nVecEps), nVec, true};

    pointInfo shadowPoint;

    double maxT = subtract(currentLight.position, sRay.origin).x / sRay.direction.x;


    int sizeOfLoop1 = scene.spheres.size(), 
        sizeOfLoop2 = scene.triangles.size(),
        sizeOfLoop3 = scene.meshes.size();

    int shapeLoopSize = sizeOfLoop1 + sizeOfLoop2 + sizeOfLoop3;


    for(int j = 0; j < shapeLoopSize; j++){

                if (j < sizeOfLoop1){
                   
                    shadowPoint = intersectSphere(sRay, scene.spheres[j].material_id, j, scene.vertex_data[scene.spheres[j].center_vertex_id - 1], scene.spheres[j].radius);
            
                    if(shadowPoint.intersectHappen){
                        if(maxT > shadowPoint.t && shadowPoint.t >= 0){
                            issShadow = true;
                            break;
                        }
                    }
                }
                else if (j >= sizeOfLoop1 && j < sizeOfLoop2){
                        
                    Triangle newTriangle = scene.triangles[j + sizeOfLoop1];
                    
                    Vec3f v0 = scene.vertex_data[newTriangle.indices.v0_id - 1];
                    Vec3f v1 = scene.vertex_data[newTriangle.indices.v1_id - 1];
                    Vec3f v2 = scene.vertex_data[newTriangle.indices.v2_id - 1];
            
                    shadowPoint = intersectTriangle(sRay, newTriangle.material_id, j + sizeOfLoop1, v0, v1, v2);
                
                    if(shadowPoint.intersectHappen){
                        if(maxT > shadowPoint.t && shadowPoint.t >= 0){
                            issShadow = true;
                            break;
                        }
                    }
                }
        
                else if (j >= sizeOfLoop2 && j < sizeOfLoop3){
                     Mesh currentMesh = scene.meshes[j + sizeOfLoop2];

                    shadowPoint = intersectMesh(sRay, currentMesh.material_id, scene, j + sizeOfLoop2, currentMesh);
            
                    if(shadowPoint.intersectHappen){
                        if(maxT > shadowPoint.t && shadowPoint.t >= 0){
                            issShadow = true;
                            break;
                        }
                    }
                }
            
            }

        return issShadow;
}


Vec3f computeColor(const Ray& ray, const pointInfo &point_info, const Camera &camera, const Scene &scene, int recurDepth){

    Vec3f color, pixelWeWant = {0,0,0};

    if(point_info.intersectHappen){

        int material_id = point_info.material_id;

        pixelWeWant = multiply(scene.ambient_light, scene.materials[material_id-1].ambient);


        int sizeOfLoop = scene.point_lights.size();
    

        for(int i = 0; i < sizeOfLoop; i++){

            PointLight currentLight = scene.point_lights[i];

            bool issShadow =  isShadow(scene, point_info, currentLight);

            pixelWeWant = calculateRadiance(currentLight, scene, ray, point_info, material_id, issShadow, pixelWeWant);

        }
        Vec3f reflection = {0,0,0};

        if(recurDepth >  0  && scene.materials[material_id-1].is_mirror){

            double ge = -2 * dotProduct(ray.direction, point_info.normalVector);
            Vec3f ge_normal = normalizeVector(add(scalarMultiply(point_info.normalVector, ge), ray.direction)); 
            Vec3f ge_epsilon = scalarMultiply(ge_normal, scene.shadow_ray_epsilon);
            
            Ray reflectionRay = { add(point_info.pointCoordinates, ge_epsilon), ge_normal, false};

            pointInfo intersectionRes = intersectionResult(reflectionRay, scene);

            reflection = computeColor(reflectionRay, intersectionRes, camera, scene, (recurDepth - 1));

            pixelWeWant = add(pixelWeWant, multiply(reflection, scene.materials[material_id - 1].mirror));

        }

    }
    else{
        pixelWeWant.x = scene.background_color.x;
        pixelWeWant.y = scene.background_color.y;
        pixelWeWant.z = scene.background_color.z;

    }

    color = pixelWeWant;

    return color;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file


    // Sample usage for reading an XML scene file
    Scene scene;

    scene.loadFromXml(argv[1]);

    int numberOfCameras = scene.cameras.size();

    for(int cameraNumber = 0; cameraNumber < numberOfCameras; cameraNumber++)
    {
        
        int numberOfLights = scene.point_lights.size();
        Camera currentCamera = scene.cameras[cameraNumber];
        int height = currentCamera.image_height;
        int width = currentCamera.image_width;
        
        int columnWidth = width / 8;

        unsigned char* image = new unsigned char [width * height * 3];
        int pixelNumber = 0;

        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                Ray ray = generateRay(i, j, currentCamera);

                pointInfo hitResult = intersectionResult(ray, scene);


                Vec3f color = computeColor(ray, hitResult, currentCamera, scene, scene.max_recursion_depth);

                if(color.x >= 255)
                    image[pixelNumber++] = 255;
                
                
                else
                    image[pixelNumber++] = round(color.x);

                if(color.y >= 255)
                    image[pixelNumber++] = 255;
                
                
                else
                    image[pixelNumber++] = round(color.y);

                if(color.z >= 255)
                    image[pixelNumber++] = 255;
                
                
                else
                    image[pixelNumber++] = round(color.z);
            }
        }

        write_ppm(currentCamera.image_name.c_str(), image, width, height);
    }

}