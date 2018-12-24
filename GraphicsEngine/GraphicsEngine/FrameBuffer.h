#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

typedef unsigned int GLuint;
typedef int GLint;

#define DEFAULT_FRAMEBUFFER_WIDTH 1024
#define DEFAULT_FRAMEBUFFER_HEIGHT 1024

#define MAX_FB_TEXTURES 10

enum TextureAttachment
{
	TA_COLOR,
	TA_DEPTH,
	TA_STENCIL
};

struct TextureConfig
{
	GLuint texture = 0;
	GLint format = 0;
	GLint type = 0;

	GLint minFilter = 0;
	GLint magFilter = 0;
	GLint wrapS = 0;
	GLint wrapT = 0;

	TextureAttachment attachment;
};

void CreateDefaultColorTexture(TextureConfig& config);
void CreateDefaultDepthTexture(TextureConfig& config);
void CreateDefaultStencilTexture(TextureConfig& config);

class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(int width, int height);
	virtual ~FrameBuffer();

	void Create();
	void Destroy();
	void Resize(int width, int height);

	void Bind();
	void UnBind();

	GLuint FBO() const { return m_fbo; }

	int Width()const { return m_width; }
	int Height()const { return m_height; }

	void CreateTexture(TextureConfig config);

	inline GLuint GetTexture(int index) const
	{
		if(index >= 0 && index < MAX_FB_TEXTURES)
		{
			return m_textures[index].texture;
		}

		return 0;
	}

private:

private:
	int m_width, m_height;

	GLuint m_fbo = 0;
	TextureConfig m_color;

	int m_colorTextureCount = 0;
	TextureConfig m_textures[MAX_FB_TEXTURES];
	int m_textureCounter = 0;
};

#endif // !__FRAMEBUFFER_H__