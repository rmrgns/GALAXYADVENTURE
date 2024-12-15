#include "star.h"
#include "game.h"


void Star::Draw(GLuint transformLoc)
{
	// emission effect
	glUniform1f(glGetUniformLocation(shaderProgramID, "emissionPower"), emissionPower);

	// texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID); // 저장된 텍스처 활성화
	glUniform1i(glGetUniformLocation(shaderProgramID, "texture1"), 0);

	// matrix
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(Matrix));

	// render
	CreateModel(vaoStar, vboStar, eboStar, model);
	glBindVertexArray(vaoStar);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Star::Update(float time)
{
	lightCycle += time;
	emissionPower = 1.25f + 0.25f * sin(lightCycle); // 1.0 ~ 1.5 사이로 변동
}
