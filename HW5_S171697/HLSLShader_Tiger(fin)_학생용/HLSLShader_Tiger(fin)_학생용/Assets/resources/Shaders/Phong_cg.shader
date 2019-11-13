Shader "HLSL/Phong_cg"
{
	Properties
	{
		u_material_ambient_color("Ambient", Color) = (0,0,0,1)
		u_material_diffuse_color("Diffuse", Color) = (0,0,0,1)
		u_material_specular_color("Specular", Color) = (0,0,0,1)
		u_material_emissive_color("Emissive", Color) = (0,0,0,1)
		u_material_specular_exponent("Specular", float) = 30
		u_base_texture("Texture", 2D) = "white" {}
	}
	SubShader
	{
		Tags { "RenderType" = "Opaque" }
		LOD 100

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			// make fog work
			#pragma multi_compile_fog

			#include "UnityCG.cginc"

			#define NUMBER_OF_LIGHTS_SUPPORTED 4
		
			float4 u_light_position[NUMBER_OF_LIGHTS_SUPPORTED]; // assume point or direction in EC in this example shader
			fixed4 u_light_ambient_color[NUMBER_OF_LIGHTS_SUPPORTED], u_light_diffuse_color[NUMBER_OF_LIGHTS_SUPPORTED], u_light_specular_color[NUMBER_OF_LIGHTS_SUPPORTED];
			float4 u_light_light_attenuation_factors[NUMBER_OF_LIGHTS_SUPPORTED]; // compute this effect only if .w != 0.0
			float3 u_light_spot_direction[NUMBER_OF_LIGHTS_SUPPORTED];//빛의 방향
			float u_light_spot_exponent[NUMBER_OF_LIGHTS_SUPPORTED];
			float u_light_spot_cutoff_angle[NUMBER_OF_LIGHTS_SUPPORTED];
			float u_light_light_on[NUMBER_OF_LIGHTS_SUPPORTED];
			float u_light_slit_count[NUMBER_OF_LIGHTS_SUPPORTED];
			

			float screen_frequency;
			float screen_width;
			int screen_draw, screen_effect;

			int wolf_shading;// 0: phong, 1: gouraud
			int spider_shading;//0: 모두다 / 1: 정반사만/ 2: 난반사만/ 3: 정반+난반+exponent 바꾸기/ 4: exponent 증가/ 5: exponent 증가/ 6: 무한관찰자

			float norm_vec_x;
			float norm_vec_y;
			float norm_vec_z;

			fixed4 u_material_ambient_color;
			fixed4 u_material_diffuse_color;
			fixed4 u_material_specular_color;
			fixed4 u_material_emissive_color;
			float u_material_specular_exponent;

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
				fixed4 v_color : COLOR;
			};


			uniform fixed4 u_global_ambient_color;	//0.2f, 0.2f, 0.2f, 1.0
			

			uniform sampler2D u_base_texture;

			uniform int u_flag_texture_mapping;
			uniform int u_flag_fog;


			#define FOG_COLOR float4(0.7, 0.7, 0.7, 1.0)
			#define FOG_NEAR_DISTANCE 350.0
			#define FOG_FAR_DISTANCE 700.0
			
			fixed4 mix(fixed4 color1, fixed4 color2, float factor) {//두 색을 섞는 함수. r = x * (1-a) + y*a
				fixed4 result;
				result.x = color1.x * (1 - factor) + color2.x * factor;
				result.y = color1.y * (1 - factor) + color2.y * factor;
				result.z = color1.z * (1 - factor) + color2.z * factor;
				return result;
			}

			fixed4 lighting_equation_textured(float3 P_EC, float3 N_EC, fixed4 base_color) {

				const float zero_f = 0.0;
				const float one_f = 1.0;
				
				fixed4 color_sum;
				float local_scale_factor, tmp_float;
				float3 L_EC;

				color_sum = u_material_emissive_color + u_global_ambient_color * base_color;
				//color_sum = u_material_emissive_color + u_global_ambient_color * u_material_ambient_color;

				for (int i = 0; i < NUMBER_OF_LIGHTS_SUPPORTED; i++) {
					if (u_light_light_on[i] == 0) { continue; }

					local_scale_factor = one_f;
					if (u_light_position[i].w != zero_f) { // point light source
						L_EC = u_light_position[i].xyz - P_EC.xyz;

						if (u_light_light_attenuation_factors[i].w != zero_f) {
							float4 tmp_vec4;

							tmp_vec4.x = one_f;
							tmp_vec4.z = dot(L_EC, L_EC);
							tmp_vec4.y = sqrt(tmp_vec4.z);
							tmp_vec4.w = zero_f;
							local_scale_factor = one_f / dot(tmp_vec4, u_light_light_attenuation_factors[i]);
						}

						L_EC = normalize(L_EC);

						// Spot light
						if (u_light_spot_cutoff_angle[i] < 180.0) { // [0.0, 90.0] or 180.0
							float spot_cutoff_angle = clamp(u_light_spot_cutoff_angle[i], zero_f, 90.0);
							float3 spot_dir = normalize(u_light_spot_direction[i]);

							tmp_float = dot(-L_EC, spot_dir);

							
							if (tmp_float >= cos(radians(spot_cutoff_angle))) {
								tmp_float = pow(tmp_float, u_light_spot_exponent[i]);
								
								//exotic effect
								if (u_light_slit_count[i] != 0) {
									float3 norm;
									norm.x = norm_vec_x;
									norm.y = norm_vec_y;
									norm.z = norm_vec_z;
									float tmp = dot(-L_EC, norm);
									if (tmp > 0.0) {
										tmp_float *= cos(u_light_slit_count[i] * 4 * acos(tmp_float));
									}
									else {
										tmp_float *= cos(u_light_slit_count[i] * acos(tmp_float));
									}
									
								}								
							}
							else 
								tmp_float = zero_f;
							
							local_scale_factor *= tmp_float;
						}
					}
					else {  // directional light source
						L_EC = normalize(u_light_position[i].xyz);

					}


					if (local_scale_factor > zero_f) {
						fixed4 local_color_sum = u_light_ambient_color[i] * u_material_ambient_color;

						tmp_float = dot(N_EC, L_EC);
						if (tmp_float > zero_f) {
							//local_color_sum += u_light_diffuse_color[i]*u_material_diffuse_color*tmp_float;
							if (spider_shading != 2) {
								local_color_sum += u_light_diffuse_color[i] * base_color*tmp_float;
							}
							if(spider_shading != 1){
								float3 H_EC = normalize(L_EC - normalize(P_EC));
								tmp_float = dot(N_EC, H_EC);
								if (tmp_float > zero_f) {
									if (spider_shading >= 3) {
										float m = 0.55f;
										float LH = dot(L_EC, H_EC);
										float NV = dot(-normalize(P_EC), N_EC);
										float NH = tmp_float;
										float NH2 = tmp_float * tmp_float;
										float VH = dot(-normalize(P_EC), H_EC);
										float NL = dot(N_EC, L_EC);
										float D = exp(-(1 - NH2) / (NH2*m*m)) / (4 * m*m*NH2*NH2);
										float G = min(1, min(2 * NH*NV / VH, 2 * NH*NL / VH));
										float n = 20.0f;
										float g = sqrt(n*n + LH * LH - 1);
										float gpc = g + LH;
										float gnc = g - LH;
										float cgpc = LH * gpc - 1;
										float cgnc = LH * gnc + 1;
										float F = 0.5f * gnc * gnc *(1 + cgpc * cgpc / (cgpc *cgpc)) / (gpc*gpc);

										if (spider_shading == 3) {
											float4 ks = { 2.0f*0.486f, 2.0f*0.433f, 2.0f*0.185f,1.0f };
											local_color_sum += u_light_specular_color[i] * ks * max(0, F*D*G / NV);
										}
										else{
											float4 ks = { 6.0f*0.486f, 6.0f*0.433f, 6.0f*0.185f,1.0f };
											local_color_sum += u_light_specular_color[i] * ks * max(0, F*D*G / NV);
										}
									}
									else {
										local_color_sum += u_light_specular_color[i] * u_material_specular_color*pow(tmp_float, u_material_specular_exponent);
									}
								}
							}							
						}
						color_sum += local_scale_factor * local_color_sum;
					}
				}

				return color_sum;
			}


			v2f vert(appdata i)
			{
				v2f o;

				o.vertex = UnityObjectToClipPos(i.v_position); // mul (UNITY_MATRIX_MV, float4 (pos, 1.0)).xyz 와 동일.
				o.v_position_EC = float4(UnityObjectToViewPos(i.v_position), 1.0);
				o.v_normal_EC = normalize(mul(UNITY_MATRIX_IT_MV, float4(i.v_normal, 0)).xyz);
				o.v_tex_coord = i.v_tex_coord;
				o.v_position_sc.x = i.v_position.x;
				o.v_position_sc.y = i.v_position.y;
				if (wolf_shading == 1)
				{
					u_material_diffuse_color[0] = 0.41f;
					u_material_diffuse_color[1] = 0.535f;
					u_material_diffuse_color[2] = 0.653f;
					u_material_diffuse_color[3] = 1.0f;

					u_material_ambient_color[0] = 0.02f;
					u_material_ambient_color[1] = 0.02f;
					u_material_ambient_color[2] = 0.04f;
					u_material_ambient_color[3] = 1.0f;

					u_material_specular_color[0] = 0.3f;
					u_material_specular_color[1] = 0.45f;
					u_material_specular_color[2] = 0.5f;
					u_material_specular_color[3] = 1.0f;

					u_material_emissive_color[0] = 0.1f;
					u_material_emissive_color[1] = 0.1f;
					u_material_emissive_color[2] = 0.1f;
					u_material_emissive_color[3] = 1.0f;

					u_material_specular_exponent = 60.f;
					
					o.v_color = lighting_equation_textured(o.v_position_EC, o.v_normal_EC, u_material_diffuse_color);
				}
				else
				{
					o.v_color = u_material_diffuse_color;
				}
				return o;
			}

			fixed4 frag(v2f i) : SV_Target
			{
				// sample the texture
				fixed4 color;

				fixed4 base_color, shaded_color;
				float fog_factor;

				if (wolf_shading == 1)
				{
					color = i.v_color;
				}
				else
				{
					if (u_flag_texture_mapping == 1)
						base_color = tex2D(u_base_texture, i.v_tex_coord);
					else
						base_color = u_material_diffuse_color;
					color = lighting_equation_textured(i.v_position_EC, normalize(i.v_normal_EC), base_color);
				}
				/*
				if (u_flag_fog == 1)
				{
					fog_factor = (FOG_FAR_DISTANCE - length(i.v_position_EC.xyz)) / (FOG_FAR_DISTANCE - FOG_NEAR_DISTANCE);
					fog_factor = clamp(fog_factor, 0.0, 1.0);
					color = mix(FOG_COLOR, shaded_color, fog_factor);
				}
				else
					color = shaded_color;
				*/

				return color;
			}
			ENDCG
		}
	}
}