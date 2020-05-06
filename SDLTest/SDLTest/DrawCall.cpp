#include "DrawCall.h"

#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Debug.h"


DrawCall::DrawCall()
{
}


DrawCall::~DrawCall()
{
}

void DrawCall::BeginDo()
{
	auto blend = material_->blendType_;
	if (blend == Blend_Alpha)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
}

void DrawCall::EndDo()
{
	auto blend = material_->blendType_;
	if (blend == Blend_Alpha)
	{
		glDisable(GL_BLEND);
	}
}

void DrawCall::Do()
{
	BeginDo();
	glUseProgram(material_->programID_);

	glBindVertexArray(vb_->vao_);
	//---vb attributes
	glBindBuffer(GL_ARRAY_BUFFER, vb_->bufferName_);
	int inx = 0;
	GLint stride = material_->CalculateStride();
	for (auto& param: material_->vsAttributeParams_)
	{
		glEnableVertexAttribArray(param->paramLocation_);
		glVertexAttribPointer(param->paramLocation_, param->num_, param->type_, param->normalized_, stride, material_->CalculateOffset(inx));
		inx += 1;
	}
	//___vb attributes end
	
	//---param update thins
	for (auto& param : material_->params_)
	{
		if (param->bNeedUpdate_)
		{
			param->UpdateValue();
		}
	}
	//___param update things end
	
	//??? 现在画了4个顶点
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
	//Unbind program
	glUseProgram(NULL);
	//___ clear things
	EndDo();

}

void DrawCall::SetMaterial(Material* material)
{
	material_ = material;
}

void DrawCall::SetVB(VertexBuffer* vb)
{
	vb_ = vb;
}

void DrawCall::SetIB(IndexBuffer* ib)
{
	ib_ = ib;
}