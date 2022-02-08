/**
 * 64kB OpenGL intro
 * Zdrojovy soubor pro texturovaci jednotku
 * @author Tomas Milet, xmilet01
 * @date 13.3.2012
 */

#include"gputextureunit.h"

void gpu_InitTextureUnit(
		SGPUTextureUnit*Unit,
		GLuint UnitUniform,
		unsigned UnitNumber){
	Unit->UnitNumber=UnitNumber;//cislo texturovaci jednotky
	Unit->UnitUniform=UnitUniform;//uniform texturovaci jednotky
}

void gpu_SetTextureUnit(SGPUTextureUnit*Unit){
	glActiveTexture(GL_TEXTURE0+Unit->UnitNumber);//aktivujeme
	glUniform1i(Unit->UnitUniform,Unit->UnitNumber);//nastavime uniform
}

