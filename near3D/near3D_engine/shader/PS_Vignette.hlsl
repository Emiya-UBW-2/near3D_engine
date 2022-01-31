// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 dif         : COLOR0;		// ディフューズカラー
	float2 texCoords0  : TEXCOORD0;	// テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 color0           : SV_TARGET0;	// 色
};


// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor;			// アルファ値等

	float		MulAlphaColor;			// カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
	float		AlphaTestRef;			// アルファテストで使用する比較値
	float2		Padding1;

	int			AlphaTestCmpMode;		// アルファテスト比較モード( DX_CMP_NEVER など )
	int3		Padding2;

	float4		IgnoreTextureColor;	// テクスチャカラー無視処理用カラー
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register(b1)
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base;
};

// プログラムとのやり取りのために使うレジスタ1
cbuffer cbMULTIPLYCOLOR_CBUFFER1 : register(b2)
{
	float2	dispsize;
}

// プログラムとのやり取りのために使うレジスタ2
cbuffer cbMULTIPLYCOLOR_CBUFFER2 : register(b3)
{
	float4	lenspos;
}

SamplerState g_DiffuseMapSampler            : register(s0);		// ディフューズマップサンプラ
Texture2D    g_DiffuseMapTexture            : register(t0);		// ディフューズマップテクスチャ

float VignetteCalc(in float2 pixel_pos) {
	float distance = sqrt(pow(pixel_pos.x, 2) + pow(pixel_pos.y, 2));
	return 1.f - pow(distance,3);
}

// main関数
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

	// テクスチャカラーの読み込み
	TextureDiffuseColor = g_DiffuseMapTexture.Sample(g_DiffuseMapSampler, CalcPos);

	// 出力カラー = テクスチャカラー * ディフューズカラー
	PSOutput.color0 = TextureDiffuseColor * PSInput.dif * VignetteCalc(pixel_pos);

	// 出力パラメータを返す
	return PSOutput;
}

