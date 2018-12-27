#include "FrameBuffer.h"

#include "OpenGL.h"

void CreateDefaultColorTexture(TextureConfig& config)
{
	config.attachment = TextureAttachment::TA_COLOR;
	config.format = GL_RGB;
	config.type = GL_UNSIGNED_BYTE;
	config.minFilter = config.magFilter = GL_LINEAR;
	config.wrapS = config.wrapT = GL_CLAMP_TO_BORDER;
}

void CreateDefaultDepthTexture(TextureConfig& config)
{
	config.attachment = TextureAttachment::TA_DEPTH;
	config.format = GL_DEPTH_COMPONENT;
	config.type = GL_FLOAT;
	config.minFilter = config.magFilter = GL_NEAREST;
	config.wrapS = config.wrapT = GL_CLAMP_TO_BORDER;
}

void CreateDefaultStencilTexture(TextureConfig& config)
{
	config.attachment = TextureAttachment::TA_STENCIL;
	config.format = GL_STENCIL_COMPONENTS;
	config.type = GL_FLOAT;
	config.minFilter = config.magFilter = GL_LINEAR;
	config.wrapS = config.wrapT = GL_CLAMP_TO_BORDER;
}

// ==========================================

FrameBuffer::FrameBuffer() : m_width(DEFAULT_FRAMEBUFFER_WIDTH), m_height(DEFAULT_FRAMEBUFFER_HEIGHT)
{
}

FrameBuffer::FrameBuffer(int width, int height) : m_width(width), m_height(height)
{
	Create();
}


FrameBuffer::~FrameBuffer()
{
	Destroy();
}

void FrameBuffer::Create()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	// TODO: Create all textures and bind them


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Destroy()
{
	glDeleteFramebuffers(1, &m_fbo);
}

void FrameBuffer::Resize(int width, int height)
{
	if(width != m_width || height != m_height)
	{
		m_width = width;
		m_height = height;

		Destroy();
		Create();
	}
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::CreateTexture(TextureConfig config)
{
	glGenTextures(1, &config.texture);
	glBindTexture(GL_TEXTURE_2D, config.texture);

	glTexImage2D(GL_TEXTURE_2D, 0, config.format, m_width, m_height, 0, config.format, config.type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT);

	GLint attachment = 0;
	switch (config.attachment)
	{
	case TA_COLOR:
		attachment = GL_COLOR_ATTACHMENT0 + m_colorTextureCount;
		++m_colorTextureCount;
		break;

	case TA_DEPTH:
		attachment = GL_DEPTH_ATTACHMENT;
		break;

	case TA_STENCIL:
		attachment = GL_STENCIL_ATTACHMENT;
		break;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, config.texture, 0);

	m_textures[m_textureCounter++] = config;
}
