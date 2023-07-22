#define _USE_MATH_DEFINES

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"



using namespace tinyxml2;
using namespace std;

double getMinOfThree(double f, double s, double t){
	return min(f, min(s, t));
}

double getMaxOfThree(double f, double s, double t){
	return max(f, max(s, t));
}

Matrix4 computeModelingTransformationMatrix(Camera* camera, Mesh & mesh, const vector<Translation*>& translationSet, const vector<Rotation*>& rotationSet, const vector<Scaling*>& scalingSet){
	Matrix4 ModelingTransMatrix = getIdentityMatrix();

	for(int i = 0; i < mesh.numberOfTransformations; i++){
		switch(mesh.transformationTypes[i]){
			case 't':{
				Translation* translation = translationSet[mesh.transformationIds[i]-1];
				double TranslationMatrix[4][4] = {{1, 0, 0, translation->tx},
												  {0, 1, 0, translation->ty},
												  {0, 0, 1, translation->tz},
												  {0, 0, 0,       1        }};
				ModelingTransMatrix = multiplyMatrixWithMatrix(TranslationMatrix, ModelingTransMatrix);
				break;
			}
			case 's':{
				Scaling* scaling = scalingSet[mesh.transformationIds[i]-1];
				double ScalingMatrix[4][4] = {{scaling->sx,		0,			0,			0},
                                    		  {	  	0,		scaling->sy,	0,			0},
                                    		  {		0,			0,		scaling->sz,	0},
                                   			  {		0,			0,			0,			1}};
				ModelingTransMatrix = multiplyMatrixWithMatrix(ScalingMatrix, ModelingTransMatrix);
				break;
			}
			case 'r':{ // Use the alternative method. Start by finding the orthonormal basis.
				Rotation* rotation = rotationSet[mesh.transformationIds[i]-1];
				Vec3 u = Vec3(rotation->ux, rotation->uy, rotation->uz, -1), v, w;
				double smallestAbsValue = getMinOfThree(abs(rotation->ux), abs(rotation->uy), abs(rotation->uz));
				
				if (smallestAbsValue == abs(rotation->ux))
            	    v = Vec3(0, -1 * rotation->uz, rotation->uy, -1);
            	else if (smallestAbsValue == abs(rotation->uy))
            	    v = Vec3(-1 * rotation->uz, 0, rotation->ux, -1);
            	else if (smallestAbsValue == abs(rotation->uz))
            	    v = Vec3(-1 * rotation->uy, rotation->ux, 0, -1);
				
				w = normalizeVec3(crossProductVec3(u,v));
				v = normalizeVec3(v);

				double matrix_M[4][4] = {{u.x,u.y,u.z,0},
                                    			  {v.x,v.y,v.z,0},
                                    			  {w.x,w.y,w.z,0},
                                    			  {0,  0,  0,  1}};

				double inverse_matrix_M[4][4] = {{u.x,v.x,w.x,0},
                                            		 {u.y,v.y,w.y,0},
                                            		 {u.z,v.z,w.z,0},
                                            		 {0,  0,  0,  1}};
				
				double rotationalMatrix[4][4] = {
					{1,			      0,			 		       0,				     		0},
                    {0,cos(rotation->angle * M_PI/180), -sin(rotation->angle * M_PI/180), 		0},
                    {0,sin(rotation->angle * M_PI/180), cos(rotation->angle * M_PI/180),		0},
                    {0,			      0,			 		       0,			 	     		1}
				};
				ModelingTransMatrix =  multiplyMatrixWithMatrix(
						   multiplyMatrixWithMatrix(inverse_matrix_M, 
						   multiplyMatrixWithMatrix(rotationalMatrix, matrix_M)), ModelingTransMatrix);
				break;
			}
		}
	}

	return ModelingTransMatrix;
}

Matrix4 computeCameraTransformationMatrix(Camera* camera){

	double M_translate[4][4] = {{1, 0, 0, -(camera->pos.x)},
                      			{0, 1, 0, -(camera->pos.y)},
                      			{0, 0, 1, -(camera->pos.z)},
                      			{0, 0, 0, 		1		}};
    double M_rotate[4][4] = {{camera->u.x, camera->u.y, camera->u.z, 0},
                      		 {camera->v.x, camera->v.y, camera->v.z, 0},
                      		 {camera->w.x, camera->w.y, camera->w.z, 0},
                      		 {		0, 		     0, 		  0,     1}};

	return multiplyMatrixWithMatrix(M_rotate, M_translate);
}

Matrix4 computePerspectiveProjectionTransformationMatrix(Camera* camera){
	double M_perspective[4][4] = {{(2*camera->near)/(camera->right-camera->left), 0, (camera->right+camera->left) / (camera->right-camera->left), 0},
                                  {0, (2*camera->near) / (camera->top - camera->bottom), (camera->top + camera->bottom) / (camera->top - camera->bottom), 0},
                                  {0, 0, -((camera->far+camera->near) / (camera->far-camera->near)), -((2*camera->far*camera->near) / (camera->far-camera->near))},
                                  {0, 0, -1, 0}};
    return Matrix4(M_perspective);
}

Matrix4 computeOrthograpicProjectionTransformationMatrix(Camera* camera){
	double M_orthograpic[4][4] = {{2/(camera->right - camera->left), 0, 0, -((camera->right + camera->left) / (camera->right - camera->left))},
                                  {0, 2/(camera->top - camera->bottom), 0, -((camera->top + camera->bottom) / (camera->top - camera->bottom))},
                               	  {0, 0, -(2/(camera->far - camera->near)), -((camera->far + camera->near) / (camera->far - camera->near))},
                                  {0, 0, 0, 1}};
    return Matrix4(M_orthograpic);
}

Matrix4 computeViewportTransformationMatrix(Camera* camera){
	double M_viewport[4][4] = {{camera->horRes/2.0,      0,            0,  (camera->horRes-1)/2.0 },
							   {      0,           camera->verRes/2.0, 0,  (camera->verRes-1)/2.0 },
							   {      0,                 0,            0.5,       0.5             },
							   {      0,                 0,            0,         1               }};

	return Matrix4(M_viewport);
}

bool isGivenTriangleBackfaceCulled(Vec3 v0, Vec3 v1, Vec3 v2){ // normal dp camera->v
	return dotProductVec3(normalizeVec3(
						  crossProductVec3(subtractVec3(v1,v0), subtractVec3(v2,v0))), 
						  					v0) 
											< 0 ? true : false;
}

bool isGivenLineVisible(double den, double num, double & t_E, double & t_L) {

    double t = num / den;
    if (den > 0) { // potentially entering
        if (t > t_L)
            return false;
        else if (t > t_E)
            t_E = t;
    }
    else if (den < 0) {  // potentially leaving
        if (t < t_E)
            return false;
        else if (t < t_L)
            t_L = t;
    }
    else if (num > 0) {  // line parallel to edge
        return false;
    }
    return true;
}

bool clipLine(Vec4& v0, Color& c0,  Vec4& v1, Color& c1) {

    bool isVisible = false;

    double t_E = 0, t_L = 1;
    double dx = v1.x - v0.x, dy = v1.y - v0.y, dz = v1.z - v0.z;
    Color dc = c1 - c0;
    double x_min = -1, x_max = 1, y_min = -1, y_max = 1, z_min = -1, z_max = 1;

	bool contFlag = true;

	for(int i = 0; i < 6; i++){
			
		if(contFlag){	
			switch(i){
				case 0: //Check for x_min side
					contFlag = isGivenLineVisible(dx, x_min-v0.x, t_E, t_L);
					break;
				case 1: //Check for x_max side
					contFlag = isGivenLineVisible(-dx, v0.x-x_max, t_E, t_L);
					break;
				case 2: //Check for y_min side
					contFlag = isGivenLineVisible(dy, y_min-v0.y, t_E, t_L);
					break;
				case 3: //Check for y_max side
					contFlag = isGivenLineVisible(-dy, v0.y-y_max, t_E, t_L);
					break;
				case 4: //Check for z_min side
					contFlag = isGivenLineVisible(dz, z_min-v0.z, t_E, t_L);
					break;
				case 5: //Check for z_max side
					contFlag = isGivenLineVisible(-dz, v0.z-z_max, t_E, t_L);

					if(contFlag){
						isVisible = true;
						if (t_L < 1){
							v1.x = v0.x + dx * t_L;
							v1.y = v0.y + dy * t_L;
							v1.z = v0.z + dz * t_L;
							c1 = c0 + dc * t_L;
						}
						if(t_E > 0){
							v0.x = v0.x + dx * t_E;
							v0.y = v0.y + dy * t_E;
							v0.z = v0.z + dz * t_E;
							c0 = c0 + dc * t_E;
						}

					}
					break;
			}
		}
		else
			break;
	}

    return isVisible;
}



void rasterizeLine(vector<vector<Color>>& image, Vec4& v0, Color& c0,  Vec4& v1, Color& c1){

	double dx = abs(v1.x - v0.x);	
    double dy = abs(v1.y - v0.y);	
    int d;	
    Color dc, c;	
    if (dy <= dx) {  // we omit the divison operation in order to improve performance. we compare slope value by taking the absolute values	
		if (v1.x < v0.x) {	
            swap(v0, v1);	
            swap(c0, c1);	
        }	
		if(v1.y < v0.y){	
			int y = v0.y;	
    		d = 2*(v0.y - v1.y) +  (v1.x - v0.x); // we can get rid of the floating point	
			c = c0;	
    		dc = (c1 - c0) / (v1.x - v0.x);	
    		for (int x = v0.x; x <= v1.x; x++) {	
    		    image[x][y] = c.round();	
    		    if (d > 0) { // choose SouthEast	
    		        d += 2*((v0.y - v1.y) - (v1.x - v0.x));	
					y--;	
    		    }	
    		    else // choose East	
    		        d += 2*(v0.y - v1.y);	
    		    	c += dc;	
			}	
		}	
		else{	
			int y = v0.y;	
    		d = 2*(v0.y - v1.y) + (v1.x - v0.x); // we can get rid of the floating point	
			c = c0;	
    		dc = (c1 - c0) / (v1.x - v0.x);	
    		for (int x = v0.x; x <= v1.x; x++) {	
    		    image[x][y] = c.round();	
    		    if (d < 0) { // choose NorthEast	
    		        d += 2*((v0.y - v1.y) + (v1.x - v0.x));	
					y++;	
    		    }	
    		    else // choose East	
    		        d += 2*(v0.y - v1.y);	
    		    c += dc;	
			}	
				
		}	
	}   	
    	
    else if (dy > dx) {	
		if (v1.y < v0.y) {	
            swap(v0, v1);	
            swap(c0, c1);	
        }	
		if(v1.x < v0.x){	
			int x = v0.x;	
        	d = 2*(v1.x - v0.x) + (v0.y - v1.y);	
        	dc = (c1 - c0) / (v1.y - v0.y);	
			c = c0;	
        	for (int y = v0.y; y <= v1.y; y++) {	
        	    image[x][y] = c.round();	
        	    if (d < 0) { 	
        	        d += 2*((v1.x - v0.x) - (v0.y - v1.y));	
					x--;	
        	    }	
        	    else	
        	        d += 2*(v1.x - v0.x);	
        	    	c += dc;	
        	}	
		}	
		else{	
        	int x = v0.x;	
        	d = 2*(v1.x - v0.x) + (v0.y - v1.y);	
        	dc = (c1 - c0) / (v1.y - v0.y);	
			c = c0;	
        	for (int y = v0.y; y <= v1.y; y++) {	
        	    image[x][y] = c.round();	
        	    if (d > 0) { 	
        	        d += 2*((v1.x - v0.x) + (v0.y - v1.y));	
					x++;	
        	    }	
        	    else	
        	        d += 2*(v1.x - v0.x);	
        	    	c += dc;	
        	}	
		}	
    }
}



void rasterizeTriangle(vector<vector<Color>>& image, Vec4& v0, Vec4& v1, Vec4& v2, const Color* c0,  const Color* c1, const Color* c2, Camera* camera){

	// first find the x_min x_max y_min y_max of the triangle as we do when we are finding the bounding box
	double nx = camera->horRes;
	double ny = camera->verRes;

	double x_min = getMinOfThree(v0.x, v1.x, v2.x);
	if(x_min > nx - 1) //make sure it is inside the plane.
		x_min = nx - 1;
	if(x_min < 0)
		x_min = 0;

	double y_min = getMinOfThree(v0.y, v1.y, v2.y);
	if(y_min > ny - 1) 
		y_min = ny - 1;
	if(y_min < 0)
		y_min = 0;

	double x_max = getMaxOfThree(v0.x, v1.x, v2.x);
	if(x_max > nx - 1) 
		x_max = nx - 1;
	if(x_max < 0)
		x_max = 0;

	double y_max = getMaxOfThree(v0.y, v1.y, v2.y);
	if(y_max > ny - 1) 
		y_max = ny - 1;
	if(y_max < 0)
		y_max = 0;

	// now implement the overall algorithm:

	for(int y = y_min; y <= y_max; y++){
		for(int x = x_min; x <= x_max; x++){ //compute barycentric coordinates
			double alpha = (x*(v1.y-v2.y) + y*(v2.x-v1.x) + v1.x*v2.y - v1.y*v2.x) / //f_{12}(x,y)
						   (v0.x*(v1.y-v2.y) + v0.y*(v2.x-v1.x) + v1.x*v2.y - v1.y*v2.x);
			double beta = (x*(v2.y-v0.y) + y*(v0.x-v2.x) + v2.x*v0.y - v2.y*v0.x) / //f_{20}(x,y)
						  (v1.x*(v2.y-v0.y) + v1.y*(v0.x-v2.x) + v2.x*v0.y - v2.y*v0.x);
			double gamma = (x*(v0.y-v1.y) + y*(v1.x-v0.x) + v0.x*v1.y - v0.y*v1.x) / //f_{01}(x,y)
						  (v2.x*(v0.y-v1.y) + v2.y*(v1.x-v0.x) + v0.x*v1.y - v0.y*v1.x);
			if(alpha >= 0 && beta >= 0 && gamma >= 0){
				Color c = *c0*alpha + *c1*beta + *c2*gamma;
				image[x][y] = c.round();
			}
		}
	}
	
}

Matrix4 VertexProcessing(Camera* camera, Matrix4 M_camera, Matrix4 M_projection, Matrix4 M_viewport, Mesh mesh_, const vector<Scaling*> scalings, const vector<Translation*> translations, const vector<Rotation*> rotations){
	return multiplyMatrixWithMatrix(M_projection, 
						multiplyMatrixWithMatrix(M_camera, 
												computeModelingTransformationMatrix(camera, mesh_, translations, rotations, scalings)));
}

void viewportTransformation(vector<Vec4>& vec, Matrix4 M_viewport){
	for(int i = 0; i < vec.size(); i++){
		vec[i] = multiplyMatrixWithVec4(M_viewport, vec[i]);
	}
}

/*
	Transformations, clipping, culling, rasterization are done here.
	You may define helper functions.
*/




void Scene::forwardRenderingPipeline(Camera *camera)
{
	// TODO: Implement this function.

	Matrix4 M_camera = computeCameraTransformationMatrix(camera);

	Matrix4 M_projection;

	if(camera->projectionType)
		M_projection = computePerspectiveProjectionTransformationMatrix(camera);
	else
		M_projection = computeOrthograpicProjectionTransformationMatrix(camera);
	
	Matrix4 M_viewport = computeViewportTransformationMatrix(camera);


	for(int i = 0; i < meshes.size(); i++){
		
		Matrix4 M_pcm = VertexProcessing(camera, M_camera, M_projection, M_viewport, *meshes[i], scalings, translations, rotations);
		
		for(int j = 0; j < meshes[i]->numberOfTriangles; j++){
			const Vec3* v0 = vertices[meshes[i]->triangles[j].getFirstVertexId()-1];
            const Vec3* v1 = vertices[meshes[i]->triangles[j].getSecondVertexId()-1];
            const Vec3* v2 = vertices[meshes[i]->triangles[j].getThirdVertexId()-1];
            const Color* c0 = colorsOfVertices[v0->colorId-1];
            const Color* c1 = colorsOfVertices[v1->colorId-1];
            const Color* c2 = colorsOfVertices[v2->colorId-1];

			Vec4 p_v0 = multiplyMatrixWithVec4(M_pcm, Vec4(v0->x, v0->y, v0->z, 1, v0->colorId)),
			 	 p_v1 = multiplyMatrixWithVec4(M_pcm, Vec4(v1->x, v1->y, v1->z, 1, v1->colorId)),
            	 p_v2 = multiplyMatrixWithVec4(M_pcm, Vec4(v2->x, v2->y, v2->z, 1, v2->colorId));

            bool backFaceCullCheck = isGivenTriangleBackfaceCulled(Vec3(p_v0.x, p_v0.y, p_v0.z, p_v0.t),
			 														Vec3(p_v1.x, p_v1.y, p_v1.z, p_v1.t),
																	Vec3(p_v2.x, p_v2.y, p_v2.z, p_v2.t));


			if ((backFaceCullCheck && cullingEnabled) == false)
				{
			
			switch(meshes[i]->type){
					case 0:{ //wireframe


						Color topColor1 = *c1,
							  topColor2 = *c1,
							  rightColor1 = *c2,
							  rightColor2 = *c2,
							  leftColor1 = *c0,
							  leftColor2 = *c0;


						Vec4 topVertex1 = p_v1 / p_v1.t,
							 topVertex2 = p_v1 / p_v1.t,
							 rightVertex1 = p_v2 / p_v2.t,
							 rightVertex2 = p_v2 / p_v2.t,
							 leftVertex1 = p_v0 / p_v0.t,
							 leftVertex2 = p_v0 / p_v0.t;



						bool is_top_visible = clipLine(topVertex1, topColor1, rightVertex1, rightColor1),
            		    	 is_right_visible = clipLine(rightVertex2, rightColor2, leftVertex1, leftColor1),
            		    	 is_left_visible = clipLine(leftVertex2, leftColor2, topVertex2, topColor2);


            		    // Viewport Transformation
						vector<Vec4> vect{topVertex1, rightVertex1, rightVertex2, leftVertex1, leftVertex2, topVertex2};
						viewportTransformation(vect, M_viewport);


						if(is_top_visible)
            		        rasterizeLine(image, vect[0], topColor1, vect[1], rightColor1);
            		    if(is_right_visible)
            		        rasterizeLine(image, vect[2], rightColor2, vect[3], leftColor1);
            		    if(is_left_visible)
            		        rasterizeLine(image, vect[4], leftColor2, vect[5], topColor2);

					break;
					}
					case 1:{ //solid

						Vec4 topVertex = p_v1 / p_v1.t,
							 rightVertex = p_v2 / p_v2.t,
							 leftVertex = p_v0 / p_v0.t;

		                // No clipping

		                // Viewport Transformation 
						// transform the vertices

						vector<Vec4> vect{leftVertex, topVertex, rightVertex};
						viewportTransformation(vect, M_viewport);

		                rasterizeTriangle(image, vect[0], vect[1], vect[2], c0, c1, c2, camera);

					break;
					}	                
			}

			}




		}

	}



}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL) {
		str = pElement->GetText();
		
		if (strcmp(str, "enabled") == 0) {
			cullingEnabled = true;
		}
		else {
			cullingEnabled = false;
		}
	}

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		// read projection type
		str = pCamera->Attribute("type");

		if (strcmp(str, "orthographic") == 0) {
			cam->projectionType = 0;
		}
		else {
			cam->projectionType = 1;
		}

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read meshes
	pElement = pRoot->FirstChildElement("Meshes");

	XMLElement *pMesh = pElement->FirstChildElement("Mesh");
	XMLElement *meshElement;
	while (pMesh != NULL)
	{
		Mesh *mesh = new Mesh();

		pMesh->QueryIntAttribute("id", &mesh->meshId);

		// read projection type
		str = pMesh->Attribute("type");

		if (strcmp(str, "wireframe") == 0) {
			mesh->type = 0;
		}
		else {
			mesh->type = 1;
		}

		// read mesh transformations
		XMLElement *pTransformations = pMesh->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh->transformationTypes.push_back(transformationType);
			mesh->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		mesh->numberOfTransformations = mesh->transformationIds.size();

		// read mesh faces
		char *row;
		char *clone_str;
		int v1, v2, v3;
		XMLElement *pFaces = pMesh->FirstChildElement("Faces");
        str = pFaces->GetText();
		clone_str = strdup(str);

		row = strtok(clone_str, "\n");
		while (row != NULL)
		{
			int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);
			
			if (result != EOF) {
				mesh->triangles.push_back(Triangle(v1, v2, v3));
			}
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		meshes.push_back(mesh);

		pMesh = pMesh->NextSiblingElement("Mesh");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}