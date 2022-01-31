// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 dif         : COLOR0;		// �f�B�t���[�Y�J���[
	float2 texCoords0  : TEXCOORD0;	// �e�N�X�`�����W
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 color0           : SV_TARGET0;	// �F
};


// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor;			// �A���t�@�l��

	float		MulAlphaColor;			// �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float		AlphaTestRef;			// �A���t�@�e�X�g�Ŏg�p�����r�l
	float2		Padding1;

	int			AlphaTestCmpMode;		// �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3		Padding2;

	float4		IgnoreTextureColor;	// �e�N�X�`���J���[���������p�J���[
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register(b1)
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base;
};

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^1
cbuffer cbMULTIPLYCOLOR_CBUFFER1 : register(b2)
{
	float2	dispsize;
}

// �v���O�����Ƃ̂����̂��߂Ɏg�����W�X�^2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3)
{
	float4	lenspos;
}

SamplerState g_DiffuseMapSampler            : register(s0);		// �f�B�t���[�Y�}�b�v�T���v��
Texture2D    g_DiffuseMapTexture            : register(t0);		// �f�B�t���[�Y�}�b�v�e�N�X�`��

float VignetteCalc(in float2 pixel_pos) {
	float distance = sqrt(pow(pixel_pos.x, 2) + pow(pixel_pos.y, 2));
	return 1.f - pow(distance,3);
}

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float4 TextureDiffuseColor;
	float2 pixel_pos;

	pixel_pos.x = (PSInput.texCoords0.x - 0.5f) * 1.15f;
	pixel_pos.y = (PSInput.texCoords0.y - 0.5f) * 1.15f;


	float2 CalcPos;
	CalcPos.x = PSInput.texCoords0.x;
	CalcPos.y = PSInput.texCoords0.y;

	// �e�N�X�`���J���[�̓ǂݍ���
	TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, CalcPos);

	// �o�̓J���[ = �e�N�X�`���J���[ * �f�B�t���[�Y�J���[
	PSOutput.color0 = TextureDiffuseColor * PSInput.dif * VignetteCalc(pixel_pos);

	// �o�̓p�����[�^��Ԃ�
	return PSOutput;
}

