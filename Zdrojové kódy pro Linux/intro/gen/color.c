#include"color.h"

const float ToGray[]={.299,.587,.114};

void color_Hue(float*Color,float Angle){
#define ASTEP 60.0
	float ActAngle;//akualni uhel pro slozku barvy
	for(int c=2;c>=0;--c){//projdeme barvy BGR poradi
		ActAngle=fmodf(Angle+(2-c)*ASTEP*2,360)/ASTEP;
		if(ActAngle>=5)Color[c]=6-ActAngle;
		else if(ActAngle>=3)Color[c]=1;
		else if(ActAngle>=2)Color[c]=ActAngle-2;
		else Color[c]=0;
	}
#undef ASTEP
}

void color_HSV(float*Color,float Angle,float Saturaion,float Value){
	color_Hue(Color,Angle);
	vector_KMul(Color,Value);
	float Gray=vector_AMul(Color,(float*)ToGray);
	for(unsigned i=0;i<3;++i)
	Color[i]=Interpolation(Gray,Color[i],Saturaion);
}

