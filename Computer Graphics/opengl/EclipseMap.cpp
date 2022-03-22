#include "EclipseMap.h"

GLuint program;
GLuint earthTexture;
GLuint heightMap;
GLuint moonTexture;

void EclipseMap::initializeMoonBuffers() {

    glGenVertexArrays(1, &moonVAO);
    glBindVertexArray(moonVAO);

    glGenBuffers(1, &moonVBO);
    glBindBuffer(GL_ARRAY_BUFFER, moonVBO);

    float x, y, z, rcosbeta, horSplit, verSplit, alpha, beta;
    for (int verticalStep = 0; verticalStep <= verticalSplitCount; verticalStep++) {
        beta = PI / 2 - (verticalStep * PI / verticalSplitCount); 
        rcosbeta = moonRadius * cosf(beta);
        z = moonRadius * sinf(beta);
        for (int horizontalStep = 0; horizontalStep <= horizontalSplitCount; horizontalStep++) {
            alpha = horizontalStep * 2 * PI / horizontalSplitCount;
            x = rcosbeta * cosf(alpha)+2600;
            y = rcosbeta * sinf(alpha)+2600;
            horSplit = (float)horizontalStep / horizontalSplitCount;
            verSplit = (float)verticalStep / verticalSplitCount;

            Vertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertex.normal = glm::normalize(glm::vec3(x/moonRadius, y/moonRadius, z/moonRadius));
            vertex.texture = glm::vec2(horSplit, verSplit);
            moonVertices.push_back(vertex);
        }
    }
    glBufferData(GL_ARRAY_BUFFER, moonVertices.size() * sizeof(Vertex), moonVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &moonEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moonEBO);

    unsigned int k1, k2;
    for(unsigned int verticalStep = 0; verticalStep < verticalSplitCount; ++verticalStep) {
        k1 = verticalStep * (horizontalSplitCount + 1);
        k2 = k1 + horizontalSplitCount + 1;
        for(unsigned int horizontalStep = 0; horizontalStep < horizontalSplitCount; ++horizontalStep, ++k1, ++k2) {
            if(verticalStep != 0) {
                moonIndices.push_back(k1);
                moonIndices.push_back(k2);
                moonIndices.push_back(k1 + 1);
            }
            if(verticalStep != (horizontalSplitCount - 1)) {
                moonIndices.push_back(k1 + 1);
                moonIndices.push_back(k2);
                moonIndices.push_back(k2 + 1);
            }
        }
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, moonIndices.size() * sizeof(int), moonIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (offsetof(Vertex, texture)));

    glEnableVertexArrayAttrib(moonVAO,0);
    glEnableVertexArrayAttrib(moonVAO,1);
    glEnableVertexArrayAttrib(moonVAO,2);
}

void EclipseMap::initializeWorldBuffers() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float x, y, z, rcosbeta, horSplit, verSplit, alpha, beta;
	for (int verticalStep = 0; verticalStep <= verticalSplitCount; verticalStep++) {
		beta = PI / 2 - (verticalStep * PI / verticalSplitCount);
		rcosbeta = radius * cosf(beta);
		z = radius * sinf(beta);
		for (int horizontalStep = 0; horizontalStep <= horizontalSplitCount; horizontalStep++) {
			alpha = horizontalStep * 2 * PI / horizontalSplitCount;
			x = rcosbeta * cosf(alpha);
			y = rcosbeta * sinf(alpha);
			horSplit = (float)horizontalStep / horizontalSplitCount;
			verSplit = (float)verticalStep / verticalSplitCount;

			Vertex vertex;
			vertex.position = glm::vec3(x, y, z);
			vertex.normal = glm::normalize(glm::vec3(x/radius, y/radius, z/radius));
			vertex.texture = glm::vec2(horSplit, verSplit);
			vertices.push_back(vertex);
		}
	}
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	int k1, k2;
	for(int verticalStep = 0; verticalStep < verticalSplitCount; ++verticalStep) {
		k1 = verticalStep * (horizontalSplitCount + 1);
		k2 = k1 + horizontalSplitCount + 1;
		for(int horizontalStep = 0; horizontalStep < horizontalSplitCount; ++horizontalStep, ++k1, ++k2) {

			if(verticalStep != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}
			if(verticalStep != (horizontalSplitCount - 1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (offsetof(Vertex, texture)));

	glEnableVertexArrayAttrib(VAO,0);
	glEnableVertexArrayAttrib(VAO,1);
	glEnableVertexArrayAttrib(VAO,2);
}

void EclipseMap::configureGeometry() {

    cameraPosition = glm::vec3(0, 4000, 4000);
    cameraDirection = glm::vec3(0.0, -1.0, -1.0);
    cameraUp = glm::vec3(0.0, 0.0, 1.0);
    cameraLeft = glm::cross(cameraUp, cameraDirection);

    cameraUp = glm::rotate(cameraUp, 0.0f, cameraLeft);
    cameraDirection = glm::rotate(cameraDirection, 0.0f, cameraLeft);

    M_model = glm::rotate(M_model, (float) glm::radians(0.0), glm::vec3(1, 0, 0));
    M_view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
    M_projection = glm::perspective(projectionAngle, aspectRatio, near, far);
    MVP = M_projection * M_view * M_model;

    initMVP = MVP;
    initM_model = M_model;
    initM_view = M_view;
    initM_projection = M_projection;

}


void EclipseMap::drawWorld() {
	
	cameraPosition += speed * cameraDirection;
	M_view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
	M_projection = glm::perspective(projectionAngle, aspectRatio, near, far);
	MVP = M_projection * M_view * M_model;

	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(uniformCameraPosition, 1, glm::value_ptr(cameraPosition));
	glUniform1f(uniformHeightFactor, heightFactor);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
void EclipseMap::drawMoon() {
	
	cameraPosition += speed * cameraDirection;
	M_model = glm::translate(M_model, glm::vec3(0, 0, 0));
	M_model = glm::rotate(M_model, 0.01f, glm::vec3(0, 0, -1));
	M_view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
	M_projection = glm::perspective(projectionAngle, aspectRatio, near, far);
	MVP = M_projection * M_view * M_model;

	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(uniformCameraPosition, 1, glm::value_ptr(cameraPosition));
	glUniform1f(uniformHeightFactor, heightFactor);
	
	glDrawElements(GL_TRIANGLES, moonIndices.size(), GL_UNSIGNED_INT, 0);

}

bool increaseHeightFactor = false; 
bool decreaseHeightFactor = false; 
bool increasePitch = false; 
bool decreasePitch= false;
bool increaseYaw = false; 
bool decreaseYaw = false; 
bool stopCamera = false; 
bool speedUp = false; 
bool speedDown = false;
bool resetPosition = false; 
bool fullscreenMode = false;

void EclipseMap::changeDisplayFormat() {
	if (fullscreenMode) {
		fullscreenMode = false;
		glfwSetWindowMonitor(window, NULL, defaultScreenWidth, defaultScreenHeight, defaultScreenWidth, defaultScreenHeight, 0);
		glViewport(0, 0, defaultScreenWidth, defaultScreenHeight);
	}
	else {
		fullscreenMode = true;
		glfwGetWindowPos(window, &defaultScreenWidth, &defaultScreenHeight);
		glfwGetWindowSize(window, &defaultScreenWidth, &defaultScreenHeight);
		auto monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode * mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 0);
		glViewport(0, 0, mode->width, mode->height);
	}
}



static void handleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {


	switch(key){
		case(GLFW_KEY_W):
			if(action == GLFW_PRESS)
				increasePitch = true;
			else if(action == GLFW_RELEASE)
				increasePitch = false;	
			break;

		case(GLFW_KEY_S):
			if(action == GLFW_PRESS)
				decreasePitch = true;
			else if(action == GLFW_RELEASE)
				decreasePitch = false;	
			break;

		case(GLFW_KEY_A):
			if(action == GLFW_PRESS)
				decreaseYaw = true;
			else if(action == GLFW_RELEASE)
				decreaseYaw = false;	
			break;

		case(GLFW_KEY_D):
			if(action == GLFW_PRESS)
				increaseYaw = true;
			else if(action == GLFW_RELEASE)
				increaseYaw = false;	
			break;

		case(GLFW_KEY_R):
			if(action == GLFW_PRESS)
				increaseHeightFactor = true;
			else if(action == GLFW_RELEASE)
				increaseHeightFactor = false;	
			break;

		case(GLFW_KEY_F):
			if(action == GLFW_PRESS)
				decreaseHeightFactor = true;
			else if(action == GLFW_RELEASE)
				decreaseHeightFactor = false;	
			break;

		case(GLFW_KEY_Y):
			if(action == GLFW_PRESS)
				speedUp = true;
			else if(action == GLFW_RELEASE)
				speedUp = false;	
			break;

		case(GLFW_KEY_H):
			if(action == GLFW_PRESS)
				speedDown = true;
			else if(action == GLFW_RELEASE)
				speedDown = false;	
			break;

		case(GLFW_KEY_X):
			if(action == GLFW_PRESS)
				stopCamera = true;
			else if(action == GLFW_RELEASE)
				stopCamera = false;	
			break;

		case(GLFW_KEY_I):
			if(action == GLFW_PRESS)
				resetPosition = true;
			else if(action == GLFW_RELEASE)
				resetPosition = false;	
			break;

		case(GLFW_KEY_ESCAPE):
			if(action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);	
			break;

		case(GLFW_KEY_P):
			if(action == GLFW_RELEASE){
				EclipseMap x;	
				x.changeDisplayFormat();
			}	
			break;

	}

}


static void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void EclipseMap::Render(char *coloredTexturePath, char *greyTexturePath, char *moonTexturePath){

	if (!glfwInit()) {
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(defaultScreenWidth, defaultScreenHeight, "CENG477 - HW3", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	glViewport(0,0, defaultScreenWidth, defaultScreenHeight);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		glfwTerminate();
		exit(-1);
	}


	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetKeyCallback(window, handleKeyPress);

	GLuint moonShaderID = initShaders("moonShader.vert", "moonShader.frag");
	initMoonColoredTexture(moonTexturePath, &mooncoloredTextureWidth, &mooncoloredTextureHeight);
	configureGeometry();
	glUseProgram(moonShaderID);

	GLuint worldShaderID = initShaders("worldShader.vert", "worldShader.frag");
	initColoredTexture(coloredTexturePath, &textureGreyWidth, &textureGreyHeight);
	initGreyTexture(greyTexturePath, &coloredTextureWidth, &coloredTextureHeight);
	
	configureGeometry();
	glUseProgram(worldShaderID);

	//SET UNIFORMS HERE

	uniformMVP = glGetUniformLocation(worldShaderID, "MVP");
	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(MVP));

	uniformHeightFactor = glGetUniformLocation(worldShaderID, "heightFactor");
	glUniform1f(uniformHeightFactor, heightFactor);

	uniformTextureOffset = glGetUniformLocation(worldShaderID, "textureOffset");
	glUniform1f(uniformTextureOffset, textureOffset);

	uniformCameraPosition = glGetUniformLocation(worldShaderID, "cameraPos");
	glUniform3fv(uniformCameraPosition, 1, glm::value_ptr(cameraPosition));

	uniformLightPosition = glGetUniformLocation(worldShaderID, "lightPos");
	glUniform3fv(uniformLightPosition, 1, glm::value_ptr(lightPos));

	uniformHeightMap = glGetUniformLocation(worldShaderID, "heightMapTexture");
	glUniform1i(uniformHeightMap, 0);

	uniformTexture = glGetUniformLocation(worldShaderID, "TexColor");
	glUniform1i(uniformTexture, 1);

	/* Enable DEPTH-TEST */
	glEnable(GL_DEPTH_TEST);

	int buffercountmoon = 0;
	int buffercountworld = 0;

	glm::mat4 tra_mat;
	glm::mat4 rot_mat;

	while (!glfwWindowShouldClose(window)) {

	glClearColor(0,0,0,1);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if(buffercountmoon == 0)
			{
				initializeMoonBuffers();
				buffercountmoon++;
			}
		glBindTexture(GL_TEXTURE_2D,moonTexture);
		glBindVertexArray(moonVAO);

		drawMoon();
		


		if(buffercountworld == 0)
			{
				initializeWorldBuffers();
				buffercountworld++;
			}

		glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, earthTexture);        
		drawWorld();

		tra_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		rot_mat = glm::rotate(glm::mat4(1.0f), -0.01f, glm::vec3(0, 0, -1));
		lightPos = glm::vec3(rot_mat*tra_mat*glm::vec4(lightPos, 1.0f));
		glUniform3fv(uniformLightPosition, 1, glm::value_ptr(lightPos));

		if (increasePitch) {
			cameraUp = glm::rotate(cameraUp, -0.01f, cameraLeft);
			cameraDirection = glm::rotate(cameraDirection,-0.01f, cameraLeft);
		}
		if (decreasePitch) {
			cameraUp = glm::rotate(cameraUp, 0.01f, cameraLeft);
			cameraDirection = glm::rotate(cameraDirection, 0.01f, cameraLeft);
		}
		if (increaseYaw) {
			cameraLeft = glm::rotate(cameraLeft, -0.01f, cameraUp);
			cameraDirection = glm::rotate(cameraDirection, -0.01f, cameraUp);
		}
		if (decreaseYaw) {
			cameraLeft = glm::rotate(cameraLeft, 0.01f, cameraUp);
			cameraDirection = glm::rotate(cameraDirection, 0.01f, cameraUp);
		}

		if (increaseHeightFactor) {
			heightFactor += 10;
			glUniform1f(uniformHeightFactor, heightFactor);
		}
		if (decreaseHeightFactor) {
			heightFactor -= 10;
			glUniform1f(uniformHeightFactor, heightFactor);
		}
		
		
		textureOffset -= 1;
		glUniform1f(uniformTextureOffset, textureOffset);
	


		if (speedUp)
			speed += 0.1;
		if (speedDown)
			speed -= 0.1;
		if (stopCamera)
			speed = 0.0;

		if (resetPosition) {
			cameraPosition = glm::vec3(0, 4000, 4000);
	        cameraDirection = glm::vec3(0.0, -1.0, -1.0);
	        cameraUp = glm::vec3(0.0, 0.0, 1.0);
	        cameraLeft = glm::cross(cameraUp, cameraDirection);
	        cameraUp = glm::rotate(cameraUp, 0.0f, cameraLeft);
	        cameraDirection = glm::rotate(cameraDirection, 0.0f, cameraLeft);
	        textureOffset = 0.0;
	        glUniform1i(uniformTextureOffset, textureOffset);
	        speed = 0;
	        heightFactor = 0.0;
	        lightPos = glm::vec3(0, 4000, 0);

	        MVP = initMVP;
    		M_model = initM_model;
    		M_view = initM_view;
    		M_projection = initM_projection;
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDisableVertexArrayAttrib(moonVAO,0);
    glDisableVertexArrayAttrib(moonVAO,1);
    glDisableVertexArrayAttrib(moonVAO,2);
    glDisableVertexArrayAttrib(VAO,0);
    glDisableVertexArrayAttrib(VAO,1);
    glDisableVertexArrayAttrib(VAO,2);

	glDeleteBuffers(1, &moonVBO);
    glDeleteBuffers(1, &moonEBO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &moonVAO);
    glDeleteVertexArrays(1, &VAO);
    
   
    glDeleteProgram(moonShaderID);
    glDeleteProgram(worldShaderID);

    // Close window
    glfwDestroyWindow(window);
    glfwTerminate();
	return;
}


void EclipseMap::initColoredTexture(const char *filename, int *w, int *h)
{
    int width, height;

    unsigned char *raw_image = nullptr;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen( filename, "rb" );
    unsigned long location = 0;
    int i = 0, j = 0;

    if ( !infile )
    {
        printf("Error opening jpeg file %s\n!", filename );
        return;
    }

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error( &jerr );
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress( &cinfo );
    /* this makes the library read from infile */
    jpeg_stdio_src( &cinfo, infile );
    /* reading the image header which contains image information */
    jpeg_read_header( &cinfo, TRUE );
    /* Start decompression jpeg here */
    jpeg_start_decompress( &cinfo );

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc( cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc( cinfo.output_width*cinfo.num_components);
    /* read one scan line at a time */
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    printf("Texture filename = %s\n", filename);
    glGenTextures(1, &earthTexture);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
    *w = width;
    *h = height;

    glGenerateMipmap(GL_TEXTURE_2D);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    free( raw_image );
    fclose( infile );

}

void EclipseMap::initGreyTexture(const char *filename, int *w, int *h)
{
    int width, height;

    unsigned char *raw_image = nullptr;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen( filename, "rb" );
    unsigned long location = 0;
    int i = 0, j = 0;

    if ( !infile )
    {
        printf("Error opening jpeg file %s\n!", filename );
        return;
    }

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error( &jerr );
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress( &cinfo );
    /* this makes the library read from infile */
    jpeg_stdio_src( &cinfo, infile );
    /* reading the image header which contains image information */
    jpeg_read_header( &cinfo, TRUE );
    /* Start decompression jpeg here */
    jpeg_start_decompress( &cinfo );

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc( cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc( cinfo.output_width*cinfo.num_components);
    /* read one scan line at a time */
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;
            
    printf("Heightmap Texture filename = %s\n", filename);
    glGenTextures(1, &heightMap);
    glBindTexture(GL_TEXTURE_2D, heightMap);

    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
    *w = width;
    *h = height;

    glGenerateMipmap(GL_TEXTURE_2D);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    free( raw_image );
    fclose( infile );

}

void EclipseMap::initMoonColoredTexture(const char *filename, int *w, int *h)
{
    int width, height;

    unsigned char *raw_image = nullptr;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen( filename, "rb" );
    unsigned long location = 0;
    int i = 0, j = 0;

    if ( !infile )
    {
        printf("Error opening jpeg file %s\n!", filename );
        return;
    }

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error( &jerr );
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress( &cinfo );
    /* this makes the library read from infile */
    jpeg_stdio_src( &cinfo, infile );
    /* reading the image header which contains image information */
    jpeg_read_header( &cinfo, TRUE );
    /* Start decompression jpeg here */
    jpeg_start_decompress( &cinfo );

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc( cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc( cinfo.output_width*cinfo.num_components);
    /* read one scan line at a time */
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;
    
    printf("Texture filename = %s\n", filename);
    glGenTextures(1, &moonTexture);
    glBindTexture(GL_TEXTURE_2D, moonTexture);
    

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
    *w = width;
    *h = height;

    glGenerateMipmap(GL_TEXTURE_2D);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    free( raw_image );
    fclose( infile );

}