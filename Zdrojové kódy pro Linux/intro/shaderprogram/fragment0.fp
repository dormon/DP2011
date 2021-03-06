#version 330
layout(location=0)out vec4 fragColor;
layout(location=1)out vec4 Buffer;
uniform sampler2D Side;
uniform sampler2D SideBump;
uniform sampler2D Down;
uniform sampler2D DownBump;
uniform sampler2D Up;
uniform sampler2D UpBump;
uniform sampler1D Sediment;
uniform sampler3D NorAmb;
uniform sampler3D Wet;
uniform sampler3D Caustic;
uniform sampler3D Light;
uniform float CausticT;
in vec3 pos;
in vec3 nor;
in vec3 coord;
in vec3 coord2;
in vec3 fLightPosition0;
in vec3 fCameraPosition;
in float dist;

uniform float SetAO;

void main(){
	vec3 FragmentToLight0=normalize(fLightPosition0-pos);
	vec3 CameraToFragment=normalize(pos-fCameraPosition);

	vec3 Normal;
	vec4 NorAmb=texture(NorAmb,coord2);
	vec3 Normala=NorAmb.xyz*2-1;
	float a=max(Normala.y,0);
	float b=max(-Normala.y,0);
	float c=max(Normala.x,0);
	float d=max(-Normala.x,0);
	float e=max(Normala.z,0);
	float f=max(-Normala.z,0);
	float cau=
	texture(Caustic,(coord*2+vec3(CausticT,0,0))).r*(c+d)+
	texture(Caustic,(coord*2+vec3(0,CausticT+.33,0))).r*(a+b)+
	texture(Caustic,(coord*2+vec3(0,0,CausticT+.66))).r*(e+f);


	float caca=cau/5;
	float wawa=(1-step(.35,pos.y));
	float qaqa=smoothstep(0,.01,.35-fCameraPosition.y);
	vec2 o=vec2(cos(caca*3.1415*2),sin(caca*3.1415*2))*.3*caca;
	o=vec2(clamp(pow(caca,1),0,1))*0.3*min(1,qaqa+wawa);

	vec3 DNormal=(texture(DownBump,coord.xz+o).xzy-.5)*a;
	vec3 UNormal=(texture(UpBump,coord.xz+o).xzy-.5)*vec3(1,-1,1)*b;
	vec3 RNormal=(texture(SideBump,coord.yz+o).zxy-.5)*(c+vec3(-1,1,1)*d);
	vec3 BNormal=(texture(SideBump,coord.xy+o).xyz-.5)*(e+vec3(1,1,-1)*f);

	vec3 SumN=normalize(DNormal+UNormal+RNormal+BNormal);
	Normal=mix(SumN,Normala,.5);
	Normal=normalize(Normal);
	vec3 t=vec3(0);
	t =(texture(Down,coord.xz+o)).xyz*a;
	t+=(texture(Up,coord.xz+o)).xyz*b;
	t+=(texture(Side,coord.yz+o)).xyz*(c+d);
	t+=(texture(Side,coord.xy+o)).xyz*(e+f);
	t+=texture(Sediment,pos.y/.3).xyz*pow(1-abs(Normala.y),.2);


	float hhh=step(pos.y,8);

	vec3 RCameraToFragment=normalize(reflect(CameraToFragment,Normal));
	float Ambient=0.1*NorAmb.w;
	float Lc=dot(Normal,FragmentToLight0);
	float Diffuse=max(Ambient,Lc);
	const float Shininess=30;
	float Specular=pow(max(0,dot(RCameraToFragment,FragmentToLight0)),Shininess)*clamp(Lc,0,1);
	float CamCam=abs(dot(Normala,CameraToFragment));
	float haha=mix(1,.5,step(.451,SetAO));
	Diffuse+=pow(cau,5)*(3+10*wawa)*(Diffuse)*t.g;
	float wet=texture(Wet,coord2).r;
	Diffuse*=max(.3,wet);
	Specular*=mix(.5+pow(1-wet,2)*2,1,wawa);
	haha=pow(haha,.5);
	vec4 color=vec4(t*Diffuse+Specular/4,1)*vec4(haha,haha,1,1);
	color*=vec4(1,1,1,1)*(1-wawa)+vec4(0.8,.8,1,1)*wawa;
	fragColor=color;
	Buffer=color;

}
 
