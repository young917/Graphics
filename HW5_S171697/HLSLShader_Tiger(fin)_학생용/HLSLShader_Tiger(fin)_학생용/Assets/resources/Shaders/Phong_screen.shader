﻿Shader "HLSL/Phong_screen"
{
	Properties
	{
		u_material_diffuse_color("Diffuse", Color) = (0,0,0,1)
	}
		SubShader
	{
		Tags { "RenderType" = "Opaque" }
		LOD 100

		Pass
		{
			//Cull OFF
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			// make fog work
			#pragma multi_compile_fog

			#include "UnityCG.cginc"

			float screen_frequency;
			float screen_width;
			int screen_draw, screen_effect;

			fixed4 u_material_diffuse_color;

			struct appdata
			{
				float3 v_position : POSITION;
				float3 v_normal : NORMAL;
				float2 v_tex_coord : TEXCOORD0;
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				float4 v_position_EC : COLORPOS;	//미리 정의되지 않은 field. 임의의 field 사용 가능
				float2 v_position_sc : SCREENEFFECT;	//미리 정의되지 않은 field. 임의의 field 사용 가능
				float3 v_normal_EC : NORMAL;
				float2 v_tex_coord : TEXCOORD0;
			};

			v2f vert(appdata i)
			{
				v2f o;

				o.vertex = UnityObjectToClipPos(i.v_position); // mul (UNITY_MATRIX_MV, float4 (pos, 1.0)).xyz 와 동일.
				o.v_position_EC = float4(UnityObjectToViewPos(i.v_position), 1.0);
				o.v_normal_EC = normalize(mul(UNITY_MATRIX_IT_MV, float4(i.v_normal, 0)).xyz);
				o.v_tex_coord = i.v_tex_coord;
				o.v_position_sc.x = i.v_position.x;
				o.v_position_sc.y = i.v_position.y;
				return o;
			}

			fixed4 frag(v2f i) : SV_Target
			{
				// sample the texture
				fixed4 color;
				float x, y;
				x = abs(i.v_position_sc.x);
				y = abs(i.v_position_sc.y);

				if (screen_effect == 1) {
					/* TO DO : Screen Effect 구현*/

					float tmp, r;
					float x_mod, y_mod;
					x_mod = x - 0.5f;
					y_mod = y - 0.5f;

					if ( screen_frequency > 1.0f && (x_mod >= -0.125f && x_mod <= 0.125f && y_mod >= -0.125f && y_mod <= 0.125f) )
					{
						float tmp = x_mod * x_mod + y_mod * y_mod;
						if (tmp <= 0.125f * 0.125f)
							discard;
					}
					else
					{
						x_mod = (x*screen_frequency) % 1.0f;
						y_mod = (y*screen_frequency) % 1.0f;
						if (x_mod < 0.0f)
							x_mod += 1.0f;
						if (y_mod < 0.0f)
							y_mod += 1.0f;

						x_mod /= screen_frequency;
						y_mod /= screen_frequency;

						r = (1.0f / 3.0f)*(1.0f / (2.0f * screen_frequency) );
						y_mod -= 4.0f * r;
						x_mod -= 3.0f * r;
						if (x_mod >= 0.0f && y_mod >= 0.0f)
						{
							tmp = (x_mod - r)*(x_mod - r) + y_mod * y_mod;
							if (tmp <= r * r)
								discard;
						}
						else if (x_mod <= 0.0f && y_mod >= 0.0f)
						{
							tmp = (x_mod + r)*(x_mod + r) + y_mod * y_mod;
							if (tmp <= r * r)
								discard;
						}
						else if (x_mod < 0.0f && y_mod < 0.0f)
						{
							tmp = -x_mod - (2.0f * r) ;
							if (tmp <= y_mod)
								discard;
						}
						else
						{
							tmp = x_mod - (2.0f * r);
							if (tmp <= y_mod)
								discard;
						}
					}
					
				}

				color = u_material_diffuse_color;
				return color;
			}
			ENDCG
		}
	}
}
