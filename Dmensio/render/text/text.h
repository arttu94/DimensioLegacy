#ifndef _TEXT_H_
#define _TEXT_H_

#include "font.h"
#include "fontshader.h"

class Text
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, XMMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);

	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	Font* m_Font;
	FontShader* m_FontShader;
	int m_screenWidth, m_screenHeight;
	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
public:
	XMMATRIX m_baseViewMatrix;
};

#endif