#version 430


in vec3 LightVector;
in vec3 CameraVector;
in vec2 textureCoordinate;
in vec3 vertexNormal;


uniform sampler2D TexColor;

out vec4 FragColor;

vec4 ambientReflectenceCoefficient = vec4(0.5, 0.5, 0.5, 1.0);
vec4 ambientLightColor = vec4(0.6, 0.6, 0.6, 1.0);
vec4 specularReflectenceCoefficient = vec4(1.0, 1.0, 1.0, 1.0);
vec4 specularLightColor = vec4(1.0, 1.0, 1.0, 1.0);
float SpecularExponent = 10;
vec4 diffuseReflectenceCoefficient = vec4(1.0, 1.0, 1.0, 1.0);
vec4 diffuseLightColor = vec4(1.0, 1.0, 1.0, 1.0);

void main() {

  vec4 textureColor = texture(TexColor, textureCoordinate);
  vec3 ambient = (ambientReflectenceCoefficient * ambientLightColor).xyz;
  float cosTheta = max(dot(vertexNormal, LightVector), 0.0f);
  vec3 diffuse = cosTheta * (diffuseLightColor * diffuseReflectenceCoefficient).xyz;
  vec3 reflection = reflect(-normalize(LightVector), vertexNormal);
  float cosAlpha = pow(max(dot(reflection, normalize(CameraVector)), 0.0f), SpecularExponent);
  vec3 specular = cosAlpha * (specularLightColor * specularReflectenceCoefficient).xyz;

  vec3 computedSurfaceColor = ambient + diffuse + specular;
  FragColor = vec4(clamp(textureColor.xyz * computedSurfaceColor, 0.0, 1.0), 1.0);
}
