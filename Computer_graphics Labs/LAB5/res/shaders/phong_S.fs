//this var comes from the vertex shader
//they are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_wPos;
varying vec3 v_wNormal;
varying vec2 v_uvs;
varying mat4 v_model;
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
uniform sampler2D normal_texture;
uniform float normalmap;
void main()
{

	//read texture
	vec4 texture = texture2D(color_texture,v_uvs);
	vec3 tex3 = texture.xyz;
	vec3 norm;

	vec4 normal_text = texture2D(normal_texture,v_uvs);
	if(normalmap == 1.0){
			norm = normalize(((normal_text.xyz*2.0)-1.0));
			norm  = (v_model * vec4( norm, 0.0)).xyz;
	}else{
			norm = normalize(v_wNormal);
	}

	vec3 Ka2 = Ka *tex3;
	vec3 Kd2 = Kd*tex3;
	vec3 Ks2 = texture.w*tex3;

	vec3 l = normalize(light_position-v_wPos);
	vec3 r = reflect(-l,norm);
	vec3 v = normalize(camera-v_wPos);
	vec3 La = Ka2*Ia;
	float ln = clamp(dot(l,norm),0,1);
	vec3 Ld = Kd2*ln*Id;
	float rv = clamp(dot(r,v),0,1);
	vec3 Ls = Ks2*pow(rv,alpha)*Is;
	vec3 color = La+Ld+Ls;

	//set the ouput color por the pixel
	gl_FragColor = vec4( color, 1.0 ) * 1.0;
}
