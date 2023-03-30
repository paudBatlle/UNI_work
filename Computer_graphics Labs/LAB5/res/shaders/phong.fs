//this var comes from the vertex shader
//they are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_wPos;
varying vec3 v_wNormal;
varying vec2 v_uvs;
//here create uniforms for all the data we need here
uniform vec3 light_position;
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform float alpha;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 camera;
uniform sampler2D color_texture;
void main()
{

	//read texture
	vec4 texture = texture2D(color_texture,v_uvs);
	vec3 tex3 = texture.xyz;
	vec3 Ka2 = Ka *tex3;
	vec3 Kd2 = Kd *tex3;
	vec3 Ks2 = Ks*tex3;
	vec3 norm = normalize(v_wNormal);
	vec3 l = normalize(light_position-v_wPos);
	vec3 r = reflect(-l,norm);
	vec3 v = normalize(camera-v_wPos);
	vec3 La = Ka2*Ia;
	vec3 Ld = Kd2*dot(l,norm)*Id;
	float rv = clamp(dot(r,v),0,1);
	vec3 Ls = Ks2*pow(rv,alpha)*Is;
	vec3 color = La+Ld+Ls;

	//set the ouput color por the pixel
	gl_FragColor = vec4( color, 1.0 ) * 1.0;
}
