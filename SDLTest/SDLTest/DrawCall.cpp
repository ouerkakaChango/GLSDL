#include "DrawCall.h"

#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderTexture.h"
#include "God.h"
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
	if (bDrawFrame_)
	{
		glViewport(0, 0, renderWidth_, renderHeight_);
	}
	if (!material_->IsStaticParameterInjected())
	{
		InjectStaticParameterToMaterial();
	}
	InjectDynamicParameterToMaterial();
}

void DrawCall::EndDo()
{
	auto blend = material_->blendType_;
	if (blend == Blend_Alpha)
	{
		glDisable(GL_BLEND);
	}

	if (bDrawFrame_)
	{
		glViewport(0, 0, GOD.windowW_, GOD.windowH_);
	}
}

void DrawCall::SimpleDoStart()
{
	if (bDrawFrame_)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rt_->frameBufferID_);
	}
	BeginDo();
	glUseProgram(material_->programID_);
	glBindVertexArray(vb_->vao_);
	//---vb attributes
	glBindBuffer(GL_ARRAY_BUFFER, vb_->bufferName_);
	int inx = 0;
	GLint stride = material_->CalculateStride();
	for (auto& param : material_->vsAttributeParams_)
	{
		glEnableVertexAttribArray(param->paramLocation_);
		glVertexAttribPointer(param->paramLocation_, param->num_, param->type_, param->normalized_, stride, material_->CalculateOffset(inx));
		inx += 1;
	}
	//___vb attributes end

	SimpleDo();
	

}

void DrawCall::SimpleDo()
{
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
}

void DrawCall::SimpleDoEnd()
{
	SimpleDo();
	glBindVertexArray(0);

	//Unbind program
	glUseProgram(NULL);
	//___ clear things
	EndDo();
	if (bDrawFrame_)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void DrawCall::Do()
{
	//???
	if (name_ == "cursorDC")
	{
		int aa = 1;
	}
	if (bDrawFrame_)
	{	
		glBindFramebuffer(GL_FRAMEBUFFER, rt_->frameBufferID_);
	}
	BeginDo();
	glUseProgram(material_->programID_);
	glBindVertexArray(vb_->vao_);
	//---vb attributes
	glBindBuffer(GL_ARRAY_BUFFER, vb_->bufferName_);
	int inx = 0;
	GLint stride = material_->CalculateStride();
	for (auto& param : material_->vsAttributeParams_)
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


	if (bDrawFrame_)
	{
		//??? 现在画了4个顶点
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
		{//debug save frame file
			static bool first = true;
			static int count = 0;
			if (name_ == "glowDC")
			{
				count += 1;
			}
			if (first && count >=10 && name_ == "glowDC")
			{
				SaveRTToFile("D:/zSaved.ppm",rt_);
				first = false;
			}
		}
	}
	else
	{
		//??? 现在画了4个顶点
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
		{//debug save frame file
			//glBindFramebuffer(GL_READ_FRAMEBUFFER, 1);
			//static bool first = true;
			//if (first)
			//{
			//	SaveRTToFile("D:/zBlur.ppm");
			//	first = false;
			//}
			//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		}
	}

	glBindVertexArray(0);

	//Unbind program
	glUseProgram(NULL);
	//___ clear things
	EndDo();
	if (bDrawFrame_)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
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

void DrawCall::SetRenderTexture(RenderTexture* rt)
{
	sure(rt != nullptr);
	rt_ = rt;
	bDrawFrame_ = true;
	renderWidth_ = rt->GetWidth();
	renderHeight_ = rt->GetHeight();
}

void DrawCall::InjectStaticParameterToMaterial()
{
	for (auto& param : material_->params_)
	{
		if (param->name_ == "RenderWidth")
		{
			static_cast<Uniform1fParam*>(param)->InjectValue(GetRenderWidth());
		}
		else if (param->name_ == "RenderHeight")
		{
			static_cast<Uniform1fParam*>(param)->InjectValue(GetRenderHeight());
		}
	}
	material_->bStaticParameterInjected_ = true;
}

//??? 可优化，提前判断Material是否需要dynamic
void DrawCall::InjectDynamicParameterToMaterial()
{
	for (auto& param : material_->params_)
	{
		if (param->name_ == "GlobalK")
		{
			static_cast<Uniform1fParam*>(param)->InjectValue(GOD.GlobalK());
		}
	}
}

float DrawCall::GetRenderWidth()
{
	if (bDrawFrame_)
	{
		return rt_->GetWidth();
	}
	else
	{
		return GOD.windowW_;
	}
}

float DrawCall::GetRenderHeight()
{
	if (bDrawFrame_)
	{
		return rt_->GetHeight();
	}
	else
	{
		return GOD.windowH_;
	}
}