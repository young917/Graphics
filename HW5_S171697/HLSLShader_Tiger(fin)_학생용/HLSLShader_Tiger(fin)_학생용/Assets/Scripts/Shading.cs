﻿using System.Collections;
using System.Collections.Generic;
using System.IO;
using System;
using UnityEngine;
using UnityEngine.UI;

public class Shading : MonoBehaviour
{

    static readonly int NUMBER_OF_LIGHT_SUPPORTED = 4;

    public struct Material_Parameters
    {
        public Vector4 ambient_color, diffuse_color, specular_color, emissive_color;// material color
        public float specular_exponent;
        public int wolf_shading_flag, spider_shading_flag;
    }

    int COORD_MC = 1;
    int COORD_WC = 2;
    int COORD_EC = 3;
    
    int Light_Spot = 1;
    int Light_Point = 2;

    public struct Light_Parameters
    {
        public int Pos_Type;  // EC/ WC/ MC = 1/2/3
        public int Light_Type;  // spot/ point = 1/2
        public int light_on;
        public Vector4 position;
        public Vector4 position_default;
        public Vector4 ambient_color, diffuse_color, specular_color;// light color
        public Vector3 spot_direction;
        public float spot_exponent;
        public float spot_cutoff_angle;// 180.0f이면 spot광원 사용 안함.
        public float slit_count;
        public Vector4 light_attenuation_factors; // produce this effect only if .w != 0.0f
    }

    public class OBJMaterial_Parameters
    {
        public float specular_exponent;
        public Vector4 ambient_color, diffuse_color, specular_color;

        public string texture_name;

        public OBJMaterial_Parameters()// 생성자
        {
            texture_name = "";

            specular_exponent = 0;
            ambient_color = Vector4.zero;
            diffuse_color = Vector4.zero;
            specular_color = Vector4.zero;
        }
        public void set_param(Material material)// 사용자 정의 shader 속성 설정
        {
            material.SetVector("u_material_ambient_color", ambient_color);
            material.SetVector("u_material_diffuse_color", diffuse_color);
            material.SetVector("u_material_specular_color", specular_color);

            material.SetFloat("u_material_specular_exponent", specular_exponent);

            if (texture_name.Equals("") == false)
            {
                Texture2D m_MainTexture = (Texture2D)Resources.Load(texture_name);
                material.SetTexture("u_base_texture", m_MainTexture);
                material.SetInt("u_flag_texture_mapping", 1);
            }
        }
    }

    Material_Parameters material_floor;
    static Light_Parameters[] light_property = new Light_Parameters[NUMBER_OF_LIGHT_SUPPORTED];
    Vector3 norm_exotic;// for exotic

    Material material_ps_default;//ps 쉐이더의 디폴트 메테리얼. 빛 등의 정보를 미리 기입한다.
    int animationFlag = 1;

    //배치된 오브젝트의 데이터 및 관리 클래스들
    Tiger tiger;
    Floor floor;
    Screen screen;
    Ben ben;
    Wolf wolf;
    Spider spider;

    public class PlayObject
    {
        protected int N_FRAME;
        protected int now_frame;

        protected Mesh[] meshes;

        protected Material_Parameters material_parameter;

        protected string object_name;
        protected GameObject gameObject;
        protected Transform Figure;

        public PlayObject(Material material_ps_default)
        {
            N_FRAME = 0;
            now_frame = 0;
        }

        //floor 오브젝트 생성 및 색상 설정
        protected void prepare_object(Material material_ps_default)
        {
            //오브젝트 생성
            GameObject gObject = GameObject.Find(object_name);
            
            //메테리얼 생성
            gameObject = gObject;
            Figure = gameObject.transform;
            set_material(material_ps_default);
        }

        public virtual void set_material(Material material_ps_default)
        {

            Material material = new Material(Shader.Find("HLSL/Phong_cg"));
            material.CopyPropertiesFromMaterial(material_ps_default);   //기본 정보(빛 등)을 가져온다.

            material.SetVector("u_material_ambient_color", material_parameter.ambient_color);
            material.SetVector("u_material_diffuse_color", material_parameter.diffuse_color);
            material.SetVector("u_material_specular_color", material_parameter.specular_color);
            material.SetVector("u_material_emissive_color", material_parameter.emissive_color);

            material.SetFloat("u_material_specular_exponent", material_parameter.specular_exponent);
            material.SetInt("spider_shading", material_parameter.spider_shading_flag);
            material.SetInt("wolf_shading", material_parameter.wolf_shading_flag);
            Figure.GetComponent<Renderer>().material = material;
        }
        
        public virtual void move()
        {
            if (N_FRAME > 1)//애니메이션
            {
                now_frame++;
                now_frame %= N_FRAME;

                Figure.GetComponent<MeshFilter>().mesh = meshes[now_frame];//다음 프레임으로 변경
            }
        }

       }

    public class Tiger : PlayObject
    {
        float rotation_angle = 0.0f;
        
        public Tiger(Material material_ps_default) : base(material_ps_default)
        {
            object_name = "Tiger";
            rotation_angle = 1f;
            prepare_object(material_ps_default);
        }

        //tiger 오브젝트 생성 및 색상 설정
       new void prepare_object(Material material_ps_default)
        {
            //Tiger 색상 데이터 설정
            material_parameter = new Material_Parameters();
            material_parameter.ambient_color = new Vector4(0.24725f, 0.1995f, 0.0745f, 1.0f);
            material_parameter.diffuse_color = new Vector4(0.75164f, 0.60648f, 0.22648f, 1.0f);
            material_parameter.specular_color = new Vector4(0.628281f, 0.555802f, 0.366065f, 1.0f);
            material_parameter.specular_exponent = 51.2f;
            material_parameter.emissive_color = new Vector4(0.1f, 0.1f, 0.1f, 1.0f);
            
            N_FRAME = 12;
            now_frame = 0;

            meshes = new Mesh[N_FRAME];

            for (int i = 0; i < N_FRAME; i++)
            {
                GameObject frame = Resources.Load("Models/Tiger/Tiger_" + i) as GameObject;
                Mesh fameMesh = frame.transform.Find("Tiger_" + i).GetComponent<MeshFilter>().sharedMesh;
                //내 생각: Tiger의 frame을 GameObject frame에 load해오고 이를 Mesh에 맞게 transform해서 fameMesh에 저장
                //그리고 meshes[i]에 저장해두기
                meshes[i] = fameMesh;
            }

            GameObject gObject = GameObject.Find(object_name);
            Figure = gObject.transform.Find("Tiger_0");
            //.Find는 hierarchy 리스트에서 찾고 transform.Find는 대상 오브젝트의 부모나 자식에서 찾음
            
            gameObject = gObject;

            set_material(material_ps_default);
            set_material_axes();
        }
       
        public override void set_material(Material material_ps_default)
        {            
            base.set_material(material_ps_default);
            Texture2D m_MainTexture = (Texture2D)Resources.Load("Models/Tiger/tiger_tex2");

            Figure.GetComponent<Renderer>().material.SetTexture("u_base_texture", m_MainTexture);
            Figure.GetComponent<Renderer>().material.SetInt("u_flag_texture_mapping", 1);
            
        }

        //Tiger 오브젝트 하단의 축
        void set_material_axes()
        {
            GameObject Axes = GameObject.Find("Axes_Tiger");
            GameObject axes_x = Axes.transform.Find("Axes-x").gameObject;
            GameObject axes_y = Axes.transform.Find("Axes-y").gameObject;
            GameObject axes_z = Axes.transform.Find("Axes-z").gameObject;

            //세 축이 다른 색상(쉐이더의 변수가 다름)을 사용하므로 다른 메테리얼을 사용해야 한다.
            Renderer axes_x_renderer = axes_x.GetComponent<Renderer>();
            Material material1 = new Material(Shader.Find("HLSL/Simple_cg"));
            material1.SetColor("_Primitive_color", new Color(1, 0, 0, 0));
            axes_x_renderer.material = material1;

            Renderer axes_y_renderer = axes_y.GetComponent<Renderer>();
            Material material2 = new Material(Shader.Find("HLSL/Simple_cg"));
            material2.SetColor("_Primitive_color", new Color(0, 1, 0, 0));
            axes_y_renderer.material = material2;

            Renderer axes_z_renderer = axes_z.GetComponent<Renderer>();
            Material material3 = new Material(Shader.Find("HLSL/Simple_cg"));
            material3.SetColor("_Primitive_color", new Color(0, 0, 1, 0));
            axes_z_renderer.material = material3;

        }

        public override void move()
        {
            base.move();            
            //위치 이동. 회전하는 물체이므로 원점을 기준으로 회전시킨다.
            gameObject.transform.RotateAround(Vector3.zero, Vector3.up, -rotation_angle);

            /*호랑이 머리에 고정된 광원
            Vector4 tiger_pos = gameObject.transform.position;
            Quaternion tiger_rotate = gameObject.transform.rotation;

            Vector4 light_pos =  light_property[3].position_default;// 호랑이 MC 좌표계에서의 위치
            Vector3 light_dir = light_property[3].spot_direction;// 호랑이 MC 좌표계에서의 벡터

            light_property[3].position = light_pos + tiger_pos;// MC를 WC로 옮기기
            light_property[3].spot_direction = tiger_rotate * light_dir;

            Vector3 tangentVector = Vector3.Cross(Vector3.down,tiger_pos);
            tangentVector.y = -1;
            light_property[3].spot_direction = tangentVector;
            //??: 호랑이가 바라보는 방향으로 spot light를 만드는 과정인 것 같다.
            */
        }

        public float getRotateAngle()
        {
            return rotation_angle;
        }
    }

    public class Floor : PlayObject
    {
        List<string> object_names;
        List<GameObject> gObjects;
        List<Transform> Figures;

        public Floor(Material material_ps_default) : base(material_ps_default)
        {
            object_names = new List<string>();
            gObjects = new List<GameObject>();
            Figures = new List<Transform>();
            for (int i = 0; i < 5; i++)
            {
                object_names.Add("Floor"+i);
            }
            prepare_object(material_ps_default);
        }

        //floor 오브젝트 생성 및 색상 설정
        private new void prepare_object(Material material_ps_default)
        {
            //floor의 색상 데이터 설정
            material_parameter = new Material_Parameters();//0.4 0.5 0.4
            material_parameter.ambient_color = new Vector4(0.05f, 0.01f, 0.01f, 1.0f);  //자체적으로 발하는 색상(어두울때 색). 자체 색상 거의 없음.
            material_parameter.diffuse_color = new Vector4(0.827f, 0.561f, 0.58f, 1.0f);//광원과 물체 사이에서 발하는 색. 보는 방량에 무관.
            material_parameter.specular_color = new Vector4(0.5f, 0.04f, 0.04f, 1.0f);//광택. 빛을 받고 카메라 방향에 영향을 받는 색.
            material_parameter.specular_exponent = 2.5f;
            material_parameter.emissive_color = new Vector4(0.0f, 0.0f, 0.0f, 1.0f);//공간의 색. 그냥 더해지는 색. 색 없음

            for (int i = 0; i < 5; i++)
            {
                GameObject gObject = GameObject.Find(object_names[i]);
                //메테리얼 생성
                gObjects.Add(gObject);
                Figures.Add(gObjects[i].transform);
            }
            set_material(material_ps_default);
        }

        public override void set_material(Material material_ps_default)
        {
            Material material = new Material(Shader.Find("HLSL/Phong_cg"));
            material.CopyPropertiesFromMaterial(material_ps_default);   //기본 정보(빛 등)을 가져온다.

            material.SetVector("u_material_ambient_color", material_parameter.ambient_color);
            material.SetVector("u_material_diffuse_color", material_parameter.diffuse_color);
            material.SetVector("u_material_specular_color", material_parameter.specular_color);
            material.SetVector("u_material_emissive_color", material_parameter.emissive_color);

            material.SetFloat("u_material_specular_exponent", material_parameter.specular_exponent);
            
            for(int i=0;i<5;i++)
                Figures[i].GetComponent<Renderer>().material = material;

        }
    }

    public class Screen : PlayObject
    {
        List<string> object_names;
        List<GameObject> gObjects;
        List<Transform> Figures;

        float frequency;
        float width;
        int screen_draw_flag, effect_flag;
        static float SCEEN_MAX_FREQUENCY = 50.0f;
        static float SCEEN_MIN_FREQUENCY = 1.0f;
        
        public Screen(Material material_ps_default) : base(material_ps_default)
        {
            object_names = new List<string>();
            gObjects = new List<GameObject>();
            Figures = new List<Transform>();

            for (int i = 0; i < 2; i++)
            {
                object_names.Add("Screen" + i);
            }

            prepare_screen(material_ps_default);
        }

        void prepare_screen(Material material_ps_default)
        {
            frequency = 1.0f;
            width = 0.125f;
            screen_draw_flag = 1;
            effect_flag = 0;

            //screen 색상 데이터 설정
            material_parameter = new Material_Parameters();
            material_parameter.diffuse_color = new Vector4(0.973f, 0.118f, 0.318f, 1.0f);//광원과 물체 사이에서 발하는 색. 보는 방향에 무관.
            
            for(int i = 0; i < 2; i++)
            {
                gObjects.Add(GameObject.Find(object_names[i]));
                Figures.Add(gObjects[i].transform.Find("default"));

            }
            set_material(material_ps_default);
            
        }

        public override void set_material(Material material_ps_default)
        {
            Material material = new Material(Shader.Find("HLSL/Phong_screen"));
          
            material.SetVector("u_material_diffuse_color", this.material_parameter.diffuse_color);

            material.SetInt("screen_draw", screen_draw_flag);
            material.SetInt("screen_effect", effect_flag);
            material.SetFloat("screen_frequency", frequency);
            material.SetFloat("screen_width", width);

            for(int i = 0; i < 2; i++)
            {
                Figures[i].GetComponent<Renderer>().material = material;
            }
           
        }

        public void changeScreenDraw()
        {
            screen_draw_flag = 1 - screen_draw_flag;

            if (screen_draw_flag == 1)
            {
                for (int i = 0; i < 2; i++)
                {
                    gObjects[i].SetActive(true);
                }
            }

            else
            {
                for (int i = 0; i < 2; i++)
                {
                    gObjects[i].SetActive(false);
                }
            }
        }

        public void changeScreenEffect()
        {
            effect_flag = 1 - effect_flag;
        }

        public void freq_up()
        {
            /* TO DO : screen frequency ++ 구현 */
            if ((effect_flag == 1) && (frequency < SCEEN_MAX_FREQUENCY))
                frequency *= 2.0f;
        }

        public void freq_down()
        {
            /* TO DO : screen frequency -- 구현 */
            if ((effect_flag == 1) && (frequency > SCEEN_MIN_FREQUENCY))
                frequency /= 2.0f;
        }

    }

    public class Wolf : PlayObject
    {
        Quaternion initial_rot;
        int wolf_draw_flag;

        public Wolf(Material material_ps_default) : base(material_ps_default)
        {
            object_name = "Wolf";
            prepare_object(material_ps_default);
        }

        new void prepare_object(Material material_ps_default)
        {
            wolf_draw_flag = 0;

            //색상 데이터 설정
            material_parameter = new Material_Parameters();// 0.75 0.6 0.2
            material_parameter.ambient_color = new Vector4(0.02f, 0.02f, 0.04f, 1.0f);
            material_parameter.diffuse_color = new Vector4(0.41f, 0.535f, 0.653f, 1.0f);
            material_parameter.specular_color = new Vector4(0.30f, 0.45f, 0.50f, 1.0f);
            material_parameter.specular_exponent = 60.2f;
            material_parameter.emissive_color = new Vector4(0.1f, 0.1f, 0.1f, 1.0f);

            N_FRAME = 17;
            now_frame = 0;

            meshes = new Mesh[N_FRAME];

            for (int i = 0; i < N_FRAME; i++)
            {
                if(i < 9)
                {
                    GameObject frame = Resources.Load("Models/Wolf/wolf_00000" + (i + 1) ) as GameObject;
                    Mesh fameMesh = frame.transform.Find("default").GetComponent<MeshFilter>().sharedMesh;
                    //내 생각: Tiger의 frame을 GameObject frame에 load해오고 이를 Mesh에 맞게 transform해서 fameMesh에 저장
                    //그리고 meshes[i]에 저장해두기
                    meshes[i] = fameMesh;
                }
                else
                {
                    GameObject frame = Resources.Load("Models/Wolf/wolf_0000" + (i + 1) ) as GameObject;
                    Mesh fameMesh = frame.transform.Find("default").GetComponent<MeshFilter>().sharedMesh;
                    //내 생각: Tiger의 frame을 GameObject frame에 load해오고 이를 Mesh에 맞게 transform해서 fameMesh에 저장
                    //그리고 meshes[i]에 저장해두기
                    meshes[i] = fameMesh;
                }
            }

            GameObject gObject = GameObject.Find(object_name);
            Figure = gObject.transform.Find("Figure");
            //.Find는 hierarchy 리스트에서 찾고 transform.Find는 대상 오브젝트의 부모나 자식에서 찾음

            gameObject = gObject;

            set_material(material_ps_default);

            initial_rot = gameObject.transform.rotation;
        }
     
        public override void set_material(Material material_ps_default)
        {
            material_parameter = new Material_Parameters();
            material_parameter.wolf_shading_flag = wolf_draw_flag;

            base.set_material(material_ps_default);
            Texture2D m_MainTexture = (Texture2D)Resources.Load("Models/Ben/MJeans1TEX_Lores");

            Figure.GetComponent<Renderer>().material.SetTexture("u_base_texture", m_MainTexture);
            Figure.GetComponent<Renderer>().material.SetInt("u_flag_texture_mapping", 1);

        }

        public void changeWolfDraw()
        {// shading 변경
            wolf_draw_flag = 1 - wolf_draw_flag;

        }
        
        float time = 0.0f;
        float v1 = 140.68115742f;
        float v2 = 105.2217634f;
        float v = 136.68115742f;
        double rot_angle = 0.0f;
        double TO_RADIAN = Math.PI / 180.0;
        int step = 1;
        public override void move()
        {// Wolf
            
            if (step == 1)
            {

                base.move();
                //5.532833/  1.59752f
                time += 0.05f;
                float hor_speed = 2.3f;
                float dist = v * time - 0.5f * 9.8f * time * time;
                float py, pz;
                float ny, nz;
                py = gameObject.transform.position[1];
                pz = gameObject.transform.position[2];
                ny = dist;
                nz = gameObject.transform.position[2] - hor_speed;
                double new_angle = Math.Atan((ny - py) / (nz - pz)) * (-1.0 / TO_RADIAN);
                if (pz - nz < 0.0f)
                {
                    new_angle += 180.0;
                }
                gameObject.transform.position = new Vector3(-250.0f, ny, nz);
                gameObject.transform.RotateAround(gameObject.transform.position, Vector3.right, (float)new_angle - (float)rot_angle);
                rot_angle = new_angle;
                v -= 0.05f * 9.8f;

                if (gameObject.transform.position[2] <= -250.0f || gameObject.transform.position[1] <= 0.0f)
                {
                    gameObject.transform.position = new Vector3(-250.0f, 0.0f, -250.0f);
                    gameObject.transform.RotateAround(gameObject.transform.position, Vector3.right, 0.0f - (float)rot_angle);
                    gameObject.transform.RotateAround(gameObject.transform.position, Vector3.up, -90.0f);
                    step = 2;
                    rot_angle = 0.0f;
                    time = 0.0f;
                    v = v2;
                }
            }
            else if (step == 2)
            {
                base.move();
                //7.8246079
                time += 0.05f;
                float hor_speed = 3.0f;
                float dist = v * time - 0.5f * 9.8f * time * time;
                float py, px;
                float ny, nx;
                py = gameObject.transform.position[1];
                px = gameObject.transform.position[0];
                ny = dist;
                nx = gameObject.transform.position[0] + hor_speed;
                double new_angle = Math.Atan((ny - py) / (nx - px)) * (1.0 / TO_RADIAN);
                if (nx - px < 0.0f)
                {
                    new_angle += 180.0;
                }
                gameObject.transform.position = new Vector3(nx, ny, -250.0f);
                gameObject.transform.RotateAround(gameObject.transform.position, Vector3.forward, (float)new_angle - (float)rot_angle);
                rot_angle = new_angle;
                v -= 0.05f * 9.8f;

                if (gameObject.transform.position[0] >= 250.0f || gameObject.transform.position[1] <= 0.0f)
                {
                    gameObject.transform.position = new Vector3(250.0f, 0.0f, -250.0f);
                    gameObject.transform.RotateAround(gameObject.transform.position, Vector3.forward, 0.0f - (float)rot_angle);
                    gameObject.transform.RotateAround(gameObject.transform.position, Vector3.up, -135.0f);
                    step = 3;
                    rot_angle = 0.0f;
                    time = 0.0f;
                    v = 60.0f;
                }
            }

            else if (step == 3)
            {
                gameObject.transform.RotateAround(Vector3.zero, new Vector3(1.0f, 0.0f, 1.0f), 2.5f);
                gameObject.transform.RotateAround(gameObject.transform.position, new Vector3(1.0f, 0.0f, 1.0f), 20.0f);

                if (gameObject.transform.position[1] <= 0.0f)
                {
                    step = 1;
                    rot_angle = 0.0f;
                    v = v1;
                    time = 0.0f;
                    gameObject.transform.position = new Vector3(-250.0f, 0.0f, 250.0f);
                    gameObject.transform.rotation = initial_rot;
                }
            }
        }
    }

    public class Spider : PlayObject
    {
        List<Vector3> norm_vecs;
        List<Vector3> unit_vecs;
        List<Vector3> jump_axes;
        int spider_draw_flag;// shading 결정

        public Spider(Material material_ps_default) : base(material_ps_default)
        {
            object_name = "Spider";
            prepare_object(material_ps_default);
        }

        new void prepare_object(Material material_ps_default)
        {
            //for move function
            norm_vecs = new List<Vector3>();
            unit_vecs = new List<Vector3>();
            jump_axes = new List<Vector3>();
            norm_vecs.Add(new Vector3(-1.0f, 0.0f, 0.0f));
            norm_vecs.Add(new Vector3(0.0f, 0.0f, -1.0f));
            norm_vecs.Add(new Vector3(1.0f, 0.0f, 0.0f));
            norm_vecs.Add(new Vector3(0.0f, 0.0f, 1.0f));

            unit_vecs.Add(new Vector3(0.0f, 1.0f, -1.0f));
            unit_vecs.Add(new Vector3(1.0f, 1.0f, 0.0f));
            unit_vecs.Add(new Vector3(0.0f, 1.0f, 1.0f));
            unit_vecs.Add(new Vector3(-1.0f, 1.0f, 0.0f));

            jump_axes.Add(new Vector3(-1.0f, 0.0f, 1.0f));
            jump_axes.Add(new Vector3(-1.0f, 0.0f, -1.0f));
            jump_axes.Add(new Vector3(1.0f, 0.0f, -1.0f));
            jump_axes.Add(new Vector3(1.0f, 0.0f, 1.0f));
            spider_draw_flag = 0;

            //색상 데이터 설정
            material_parameter = new Material_Parameters();// 0.75 0.6 0.2
            material_parameter.ambient_color = new Vector4(0.24725f, 0.1995f, 0.0745f, 1.0f);
            material_parameter.diffuse_color = new Vector4(0.71f, 0.42f, 0.125f, 1.0f);
            material_parameter.specular_color = new Vector4(0.628281f, 0.555802f, 0.366065f, 1.0f);
            material_parameter.specular_exponent = 70.2f;
            material_parameter.emissive_color = new Vector4(0.1f, 0.1f, 0.1f, 1.0f);


            N_FRAME = 16;
            now_frame = 0;

            meshes = new Mesh[N_FRAME];

            for (int i = 0; i < N_FRAME; i++)
            {
                if (i < 9)
                {
                    GameObject frame = Resources.Load("Models/Spider/spider__00000" + (i + 1) ) as GameObject;
                    Mesh fameMesh = frame.transform.Find("default").GetComponent<MeshFilter>().sharedMesh;
                    meshes[i] = fameMesh;
                }
                else
                {
                    GameObject frame = Resources.Load("Models/Spider/spider__0000" + (i + 1) ) as GameObject;
                    Mesh fameMesh = frame.transform.Find("default").GetComponent<MeshFilter>().sharedMesh;
                    meshes[i] = fameMesh;
                }
            }

            GameObject gObject = GameObject.Find(object_name);
            Figure = gObject.transform.Find("Figure");
            //.Find는 hierarchy 리스트에서 찾고 transform.Find는 대상 오브젝트의 부모나 자식에서 찾음

            gameObject = gObject;

            set_material(material_ps_default);
        }
        public override void set_material(Material material_ps_default)
        {
            material_parameter = new Material_Parameters();
            material_parameter.spider_shading_flag = spider_draw_flag;

            base.set_material(material_ps_default);

            Texture2D m_MainTexture = (Texture2D)Resources.Load("Models/Ben/Nail_Hand_01_Lores");
            Figure.GetComponent<Renderer>().material.SetTexture("u_base_texture", m_MainTexture);
            Figure.GetComponent<Renderer>().material.SetInt("u_flag_texture_mapping", 1);

        }

        public void changeSpiderDraw()
        {// shading 변경

            spider_draw_flag = (spider_draw_flag + 1) % 5;

        }

      
        Vector3 direction = new Vector3(1.0f, 1.0f, 0.0f);
        Vector3 tmpdir = new Vector3(1.0f, 1.0f, 0.0f);
        int wall_num = 3;
        int step = 2;
        float speed = 20.0f;
        double rot_angle = -90.0;
        double r, theta = 1.0;
        double TO_RADIAN = Math.PI / 180.0;

        public override void move()
        {// Spider
            
            //1. direction으로 움직이기
            if (step == 1)
            {
                base.move();

                gameObject.transform.position = gameObject.transform.position + direction;

                //spot light 방향 구하기
                light_property[3].spot_direction = tmpdir;

                if ( (wall_num == 3 || wall_num == 1) && ( gameObject.transform.position[0] >= -5.0f && gameObject.transform.position[0] <= 5.0f) )
                {
                    gameObject.transform.position = new Vector3(0.0f, 400.0f, gameObject.transform.position[2]);
                    step = 2;
                }
                else if ((wall_num == 2 || wall_num == 0) && (gameObject.transform.position[2] >= -5.0f && gameObject.transform.position[2] <= 5.0f) )
                {
                    gameObject.transform.position = new Vector3( gameObject.transform.position[0], 400.0f, 0.0f);
                    step = 2;
                }
            }
            //2. 중심에 옴 + 움직이면서 회전하기 + 점프하기 전에 밑에 바라보도록 돌리고 + 점프하기 전에 돌리기
            if(step == 2)
            {
                base.move();

                r = 30.0 + 30.0 * theta;
                theta += 0.06;
                float nx, ny;
                float px, py;
                nx = (float)( r * Math.Cos(theta));
                ny = (float)( r * Math.Sin(theta));
                py = gameObject.transform.position[1] - 400.0f;
                if (unit_vecs[wall_num][0] == 0.0f)
                {
                    px = gameObject.transform.position[2] * unit_vecs[wall_num][2];
                    tmpdir = new Vector3(0.0f, ny - py, (nx - px) * unit_vecs[wall_num][2]);
                }
                else
                {
                    px = gameObject.transform.position[0] * unit_vecs[wall_num][0];
                    tmpdir = new Vector3( (nx-px) * unit_vecs[wall_num][0] , ny - py, 0.0f);
                }
                tmpdir.Normalize();
                light_property[3].spot_direction = tmpdir;
                // new_rotation angle 구하기

                double new_angle = Math.Atan( (ny - py) / (nx - px) ) * (-1.0/ TO_RADIAN);
                if(( nx - px ) < 0.0)
                {
                    new_angle += 180.0;
                }
                // 물체 돌리고 위치 변화
                gameObject.transform.RotateAround(gameObject.transform.position, norm_vecs[wall_num], (float)new_angle - (float)rot_angle);
                rot_angle = new_angle;
                gameObject.transform.position = gameObject.transform.position + new Vector3((nx - px) * unit_vecs[wall_num][0], (ny-py) * unit_vecs[wall_num][1], (nx - px) * unit_vecs[wall_num][2]);

                // 점프하기 직전
                if ( wall_num == 3 && gameObject.transform.position[0] >= 400.0f  )//3->2
                {
                    gameObject.transform.position = new Vector3(400.0f, gameObject.transform.position[1], gameObject.transform.position[2]);
                    step = 3;
                    gameObject.transform.RotateAround(gameObject.transform.position, norm_vecs[wall_num], 90.0f - (float)rot_angle);
                    gameObject.transform.RotateAround(gameObject.transform.position, Vector3.down, -45.0f);
                    rot_angle = 90.0;
                }
                else if ( wall_num == 1 && gameObject.transform.position[0] <= -400.0f )//1->0
                {
                    gameObject.transform.position = new Vector3( -400.0f, gameObject.transform.position[1], gameObject.transform.position[2]);
                    step = 3;
                    gameObject.transform.RotateAround(gameObject.transform.position, norm_vecs[wall_num], 90.0f - (float)rot_angle);
                    rot_angle = 90.0;
                    gameObject.transform.RotateAround(new Vector3(-400.0f, 0.0f, 400.0f), Vector3.down, -45.0f);
                }
                else if ( wall_num == 0 && gameObject.transform.position[2] >= 400.0f )//0->3
                {
                    gameObject.transform.position = new Vector3(gameObject.transform.position[2], gameObject.transform.position[1], 400.0f);
                    step = 3;
                    gameObject.transform.RotateAround(gameObject.transform.position, norm_vecs[wall_num], 90.0f - (float)rot_angle);
                    rot_angle = 90.0;
                    gameObject.transform.RotateAround(new Vector3(400.0f, 0.0f, 400.0f), Vector3.down, -45.0f);
                }
                else if( wall_num == 2 && gameObject.transform.position[2] <= -400.0f )//2->1
                {
                    gameObject.transform.position = new Vector3(gameObject.transform.position[2], gameObject.transform.position[1], -400.0f);
                    step = 3;
                    gameObject.transform.RotateAround(gameObject.transform.position, norm_vecs[wall_num], 90.0f - (float)rot_angle);
                    rot_angle = 90.0;
                    gameObject.transform.RotateAround(new Vector3(-400.0f, 0.0f, -400.0f), Vector3.down, -45.0f);
                }

            }
            
            //3. 점프하기: 점프하고 나면 도착면에 붙도록 돌리기
            if (step == 3)
            {
                gameObject.transform.RotateAround( new Vector3(0.0f,800.0f,0.0f) , jump_axes[wall_num] , -5.0f );

                tmpdir = Vector3.Cross( jump_axes[wall_num], -gameObject.transform.position + new Vector3(0.0f,800.0f,0.0f));
                light_property[3].spot_direction = tmpdir;

                float x, y, z;
                x = gameObject.transform.position[0];
                y = gameObject.transform.position[1];
                z = gameObject.transform.position[2];

                if(wall_num == 0 && (x <= -400.0f && z <= -400.0f) )//0->3
                {
                    x = -400.0f;
                    z = -400.0f;
                    step = 4;
                    gameObject.transform.position = new Vector3(x, y, z);
                    gameObject.transform.RotateAround(new Vector3(-400.0f, 0.0f, -400.0f), Vector3.down, -45.0f);
                }
                else if( wall_num == 1 && (x >= 400.0f && z <= -400.0f) )//1->0
                {
                    x = 400.0f;
                    z = -400.0f;
                    step = 4;
                    gameObject.transform.position = new Vector3(x, y, z);
                    gameObject.transform.RotateAround(new Vector3( 400.0f, 0.0f, -400.0f), Vector3.down, -45.0f);
                }
                if( wall_num == 2 && (x >= 400.0f && z >= 400.0f ))//2->1
                {
                    x = 400.0f;
                    z = 400.0f;
                    step = 4;
                    gameObject.transform.position = new Vector3(x, y, z);
                    gameObject.transform.RotateAround(new Vector3( 400.0f, 0.0f, 400.0f), Vector3.down, -45.0f);
                }
                else if( wall_num == 3 && (z >= 400.0f && x<= -400.0f) )//3->2
                {
                    x = -400.0f;
                    z = 400.0f;
                    step = 4;
                    gameObject.transform.position = new Vector3(x, y, z);
                    gameObject.transform.RotateAround(new Vector3( -400.0f, 0.0f, 400.0f), Vector3.down, -45.0f);
                }
            }

            //4. 도착해서 direction 설정해주기 + direction 방향으로 돌리기
            if (step == 4)
            {
                base.move();

                wall_num = (wall_num - 1) % 4;
                if (wall_num < 0)
                    wall_num += 4;

                double new_angle;
                gameObject.transform.localEulerAngles = new Vector3 (-90.0f, gameObject.transform.localEulerAngles[1], gameObject.transform.localEulerAngles[2]);

                if (wall_num == 3)
                {
                    direction = new Vector3(0.0f - gameObject.transform.position[0], 400.0f - gameObject.transform.position[1], 0.0f);
                    direction /= speed;
                    Vector3 tmpdir = new Vector3(direction[0] * unit_vecs[wall_num][0], direction[1] * unit_vecs[wall_num][1], direction[2] * unit_vecs[wall_num][2]);
                    new_angle = Math.Atan(tmpdir[1] / tmpdir[0]) *(-1.0 / TO_RADIAN );
                    if (tmpdir[0] < 0.0)
                        new_angle += 180.0;
                    gameObject.transform.RotateAround(gameObject.transform.position, norm_vecs[wall_num], (float)new_angle - (float)rot_angle );

                }
                else if (wall_num == 2)
                {
                    direction = new Vector3(0.0f , 400.0f - gameObject.transform.position[1], 0.0f - gameObject.transform.position[2]);
                    direction /= speed;
                    Vector3 tmpdir = new Vector3(direction[0] * unit_vecs[wall_num][0], direction[1] * unit_vecs[wall_num][1], direction[2] * unit_vecs[wall_num][2]);
                    new_angle = Math.Atan(tmpdir[1] / tmpdir[2]) * (-1.0 / TO_RADIAN);
                    if (tmpdir[2] < 0.0)
                        new_angle += 180.0;
                    gameObject.transform.RotateAround(gameObject.transform.position, norm_vecs[wall_num], (float)new_angle - (float)rot_angle);
                }
                else if (wall_num == 1)
                {
                    direction = new Vector3(0.0f - gameObject.transform.position[0], 400.0f - gameObject.transform.position[1], 0.0f );
                    direction /= speed;
                    Vector3 tmpdir = new Vector3(direction[0] * unit_vecs[wall_num][0], direction[1] * unit_vecs[wall_num][1], direction[2] * unit_vecs[wall_num][2]);
                    new_angle = Math.Atan(tmpdir[1] / tmpdir[0]) * (-1.0 / TO_RADIAN);
                    if (tmpdir[0] < 0.0)
                        new_angle += 180.0;
                    gameObject.transform.RotateAround(gameObject.transform.position, norm_vecs[wall_num], (float)new_angle - (float)rot_angle);
                }
                else
                {
                    direction = new Vector3(0.0f , 400.0f - gameObject.transform.position[1], 0.0f - gameObject.transform.position[2]);
                    direction /= speed;
                    Vector3 tmpdir = new Vector3(direction[0] * unit_vecs[wall_num][0], direction[1] * unit_vecs[wall_num][1], direction[2] * unit_vecs[wall_num][2]);
                    new_angle = Math.Atan(tmpdir[1] / tmpdir[2]) * (-1.0 / TO_RADIAN);
                    if (tmpdir[2] < 0.0)
                        new_angle += 180.0;
                    gameObject.transform.RotateAround(gameObject.transform.position, norm_vecs[wall_num], (float)new_angle - (float)rot_angle);
                    
                }
                rot_angle = new_angle;
                step = 1;
                theta = 1.0f;
            }
            Vector4 spider_pos = gameObject.transform.position;
            Quaternion spider_rotate = gameObject.transform.rotation;

            Vector4 light_pos = light_property[3].position_default;// MC 좌표계에서의 위치
            Vector3 light_dir = light_property[3].spot_direction;// MC 좌표계에서의 벡터

            light_property[3].position = light_pos + spider_pos;// MC를 WC로 옮기기
            /*light_property[3].spot_direction = spider_rotate * light_dir;

            Vector3 tangentVector = Vector3.Cross(Vector3.down, spider_pos);
            tangentVector.y = -1;
            light_property[3].spot_direction = tangentVector;*/

        }
    }
    public class Ben : PlayObject
    {        
        SortedDictionary<string, OBJMaterial_Parameters> readed_materials;
        
        float rotation_angle = 1.0f;
        int jump_flag = 0;
        
        public Ben(Material material_ps_default) : base(material_ps_default)
        {
            object_name = "Ben";
            prepare_object(material_ps_default);
        }

        void read_material_ben()
        {
            readed_materials = new SortedDictionary<string, OBJMaterial_Parameters>();

            string rootPath = "Models/Ben/";
            //ben obj 의 material 정보가 담긴 파일.
            string filename = rootPath + "ben_mat";
            TextAsset txt = (TextAsset)Resources.Load(filename, typeof(TextAsset));

            string allText = txt.text;
            char[] stringSeparators = new char[] { '\r', '\n' };
            string[] lines = allText.Split(stringSeparators);

            string nowName = "";
            OBJMaterial_Parameters nowParameter = null;
            //파일을 한 줄별로 읽는 형식
            foreach (string s in lines)
            {
                if (s.StartsWith("newmtl"))   //새로운 material
                {
                    //이전이 있다면
                    if (nowName.Equals("") == false)
                    {
                        readed_materials.Add(nowName, nowParameter);
                    }

                    nowName = s.Substring(7);
                    // Debug.Log("nowname : " + nowName);
                    nowParameter = new OBJMaterial_Parameters();

                }
                else if (s.StartsWith("Ns"))
                {
                    float f = Convert.ToSingle(s.Substring(3));
                    nowParameter.specular_exponent = f;
                }
                else if (s.StartsWith("Ka"))
                {
                    string[] split = s.Substring(3).Split(' ');
                    nowParameter.ambient_color = new Vector4(Convert.ToSingle(split[0]), Convert.ToSingle(split[1]), Convert.ToSingle(split[2]));

                }
                else if (s.StartsWith("Kd"))
                {
                    string[] split = s.Substring(3).Split(' ');
                    nowParameter.diffuse_color = new Vector4(Convert.ToSingle(split[0]), Convert.ToSingle(split[1]), Convert.ToSingle(split[2]));

                }
                else if (s.StartsWith("Ks"))
                {
                    string[] split = s.Substring(3).Split(' ');
                    nowParameter.specular_color = new Vector4(Convert.ToSingle(split[0]), Convert.ToSingle(split[1]), Convert.ToSingle(split[2]));

                }
                else if (s.StartsWith("map_Kd"))
                {
                    string texname = s.Substring(7);
                    texname = texname.Remove(texname.Length - 4);
                    nowParameter.texture_name = rootPath + texname;
                }


            }
    ;
            if (nowName.Equals("") == false)
            {
                readed_materials.Add(nowName, nowParameter);
            }

        }

        new void prepare_object(Material material_ps_default)
        {
            read_material_ben();
            N_FRAME = 30;
            now_frame = 0;

            meshes = new Mesh[N_FRAME];
            
            for (int i = 0; i < N_FRAME; i++)
            {
                GameObject frame = Resources.Load("Models/Ben/ben_" + i.ToString("D2")) as GameObject;                
                Mesh mesh = frame.transform.Find("Figure_1").GetComponent<MeshFilter>().sharedMesh;
                meshes[i] = mesh;                
            }
            
            GameObject gObject = GameObject.Find(object_name);
            Figure = gObject.transform.Find("Figure");
            
            set_material(material_ps_default);
            gameObject = gObject;
        }

        public override void set_material(Material material_ps_default)
        {
            
            Material[] materials = Figure.GetComponent<Renderer>().materials;

            for (int i = 0; i < materials.Length; i++)
            {
                string matName = materials[i].name;
                if (matName.Contains("(Instance)"))
                {
                    matName = matName.Split(' ')[0];
                }
                if (matName.Contains("hlsl/"))
                {
                    matName = matName.Substring(5);
                }
                OBJMaterial_Parameters param = readed_materials[matName];

                Material material = new Material(Shader.Find("HLSL/Phong_cg"));
                material.CopyPropertiesFromMaterial(material_ps_default);

                param.set_param(material);

                material.name = "hlsl/" + matName;
                materials[i] = material;
            }

            Figure.GetComponent<Renderer>().materials = materials;    
        }

        public override void move()
        {
            base.move();
           
            GameObject ben = gameObject;

            //위치 이동. 회전하는 물체이므로 원점으로 돌린 후 회전, 평행이동 한다.
            if (jump_flag == 1)
            {
                /* TO DO : Ben jump 구현 */
            }
            else
            {
                Vector3 pos = gameObject.transform.position;
                pos.y = 0;
                gameObject.transform.position = pos;
            }
            gameObject.transform.RotateAround(Vector3.zero, Vector3.up, -rotation_angle);
        }

        public void jump()
        {
            jump_flag = 1 - jump_flag;
        }

        public void back()
        {
            /* TO DO : Ben Back 구현 */
        }
    }

    void initialize_lights_and_material()
    {
        material_ps_default = new Material(Shader.Find("HLSL/Phong_cg"));
        //기본 메테리얼을 빛에 대하여 초기화.

        material_ps_default.SetVector("u_global_ambient_color", new Vector4(0.2f, 0.2f, 0.2f, 1.0f));

        float[] light_on = new float[NUMBER_OF_LIGHT_SUPPORTED];
        Vector4[] position = new Vector4[NUMBER_OF_LIGHT_SUPPORTED];
        Vector4[] ambient_color = new Vector4[NUMBER_OF_LIGHT_SUPPORTED];
        Vector4[] diffuse_color = new Vector4[NUMBER_OF_LIGHT_SUPPORTED];
        Vector4[] specular_color = new Vector4[NUMBER_OF_LIGHT_SUPPORTED];
        Vector4[] spot_direction = new Vector4[NUMBER_OF_LIGHT_SUPPORTED];
        float[] spot_exponent = new float[NUMBER_OF_LIGHT_SUPPORTED];
        float[] spot_cutoff_angle = new float[NUMBER_OF_LIGHT_SUPPORTED];
        Vector4[] light_attenuation_factors = new Vector4[NUMBER_OF_LIGHT_SUPPORTED];
        float[] slit_count = new float[NUMBER_OF_LIGHT_SUPPORTED];


        for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++)
        {
            light_on[i] = 0.0f;//기본적으로 불이 꺼져 있음
            position[i] = new Vector4(0.0f, 0.0f, 1.0f, 0.0f);
            ambient_color[i] = new Vector4(0.0f, 0.0f, 0.0f, 1.0f);
            if (i == 0)
            {
                diffuse_color[i] = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
                specular_color[i] = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
            }
            else
            {
                diffuse_color[i] = new Vector4(0.0f, 0.0f, 0.0f, 1.0f);
                specular_color[i] = new Vector4(0.0f, 0.0f, 0.0f, 1.0f);
            }

            spot_direction[i] = new Vector3(0.0f, 0.0f, -1.0f);
            spot_exponent[i] = 0.0f;
            slit_count[i] = 0.0f;
            spot_cutoff_angle[i] = 180.0f;
            light_attenuation_factors[i] = new Vector4(1.0f, 0.0f, 0.0f, 0.0f);
        }


        material_ps_default.SetFloatArray("u_light_light_on", light_on);
        material_ps_default.SetVectorArray("u_light_position", position);
        material_ps_default.SetVectorArray("u_light_ambient_color", ambient_color);
        material_ps_default.SetVectorArray("u_light_diffuse_color", diffuse_color);
        material_ps_default.SetVectorArray("u_light_specular_color", specular_color);
        material_ps_default.SetVectorArray("u_light_spot_direction", spot_direction);

        material_ps_default.SetFloatArray("u_light_spot_exponent", spot_exponent);
        material_ps_default.SetFloatArray("u_light_spot_cutoff_angle", spot_cutoff_angle);
        material_ps_default.SetVectorArray("u_light_light_attenuation_factors", light_attenuation_factors);
        material_ps_default.SetFloatArray("u_light_slit_count", slit_count);


        material_ps_default.SetVector("u_material_ambient_color", new Vector4(0.2f, 0.2f, 0.2f, 1.0f));
        material_ps_default.SetVector("u_material_diffuse_color", new Vector4(0.8f, 0.8f, 0.8f, 1.0f));
        material_ps_default.SetVector("u_material_specular_color", new Vector4(0.0f, 0.0f, 0.0f, 1.0f));
        material_ps_default.SetVector("u_material_emissive_color", new Vector4(0.0f, 0.0f, 0.0f, 1.0f));

        material_ps_default.SetFloat("u_material_specular_exponent", 0.0f);

        material_ps_default.SetInt("u_flag_texture_mapping", 0);
        material_ps_default.SetInt("u_flag_fog", 0);
        material_ps_default.SetInt("wolf_shading", 0);
        material_ps_default.SetInt("spider_shading", 0);

    }

    //광원 정보를 쉐이더에 입력.
    void set_light(Material mat)
    {
        float[] property_lighton,property_spot_exponent, property_slit_count, property_cutoff_angle;
        Vector4[] property_ambient, property_diffuse, property_specular, property_position;
        Vector4[] property_spot_dir;

        //기존에 적혀있는 property
        property_lighton = mat.GetFloatArray("u_light_light_on");
        property_slit_count = mat.GetFloatArray("u_light_slit_count");
        property_ambient = mat.GetVectorArray("u_light_ambient_color");
        property_diffuse = mat.GetVectorArray("u_light_diffuse_color");
        property_specular = mat.GetVectorArray("u_light_specular_color");
        property_position = mat.GetVectorArray("u_light_position");
         property_spot_dir = mat.GetVectorArray("u_light_spot_direction");
        property_spot_exponent = mat.GetFloatArray("u_light_spot_exponent");
        property_cutoff_angle = mat.GetFloatArray("u_light_spot_cutoff_angle");

        Camera cam = GetComponent<Camera>();
        Matrix4x4 ViewMatrix = cam.worldToCameraMatrix;
        //mat3 형식으로 사용
        Matrix4x4 ViewMatrix_mat3 = ViewMatrix;
        ViewMatrix_mat3[0, 3] = 0;
        ViewMatrix_mat3[1, 3] = 0;
        ViewMatrix_mat3[2, 3] = 0;
        ViewMatrix_mat3[3, 3] = 0;
        ViewMatrix_mat3[3, 2] = 0;
        ViewMatrix_mat3[3, 1] = 0;
        ViewMatrix_mat3[3, 0] = 0;

        for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++)
        {
            //공통
            property_lighton[i] = light_property[i].light_on;
            property_slit_count[i] = light_property[i].slit_count;
            property_ambient[i] = light_property[i].ambient_color;
            property_diffuse[i] = light_property[i].diffuse_color;
            property_specular[i] = light_property[i].specular_color;

            if (light_property[i].Pos_Type == COORD_EC)//현재 적혀있는 좌표계가 EC, 바로 삽입
            {
                property_position[i] = light_property[i].position;

                if (light_property[i].Light_Type == Light_Spot)
                {
  
                    property_spot_dir[i] = light_property[i].spot_direction;
                    property_spot_exponent[i] = light_property[i].spot_exponent;
                    property_cutoff_angle[i] = light_property[i].spot_cutoff_angle;
                }
            }
            else if (light_property[i].Pos_Type == COORD_MC || light_property[i].Pos_Type == COORD_WC)
            {//현재 적혀있는 좌표계가 WC, view matrix를 pos, vector에 곱해야 한다
                Vector4 position_EC = ViewMatrix * new Vector4(light_property[i].position[0], light_property[i].position[1],
                                                            light_property[i].position[2], light_property[i].position[3]);
                property_position[i] = position_EC;


                if (light_property[i].Light_Type == Light_Spot)
                {
                    Vector4 direction_EC_tmp = ViewMatrix_mat3 * new Vector4(light_property[i].spot_direction[0], light_property[i].spot_direction[1],
                                                                                light_property[i].spot_direction[2], 0);
                    Vector3 direction_EC = new Vector3(direction_EC_tmp.x, direction_EC_tmp.y, direction_EC_tmp.z);
                    property_spot_dir[i] = direction_EC;
                    property_spot_exponent[i] = light_property[i].spot_exponent;
                    property_cutoff_angle[i] = light_property[i].spot_cutoff_angle;
                }
            }
        }
        //위치좌표
        mat.SetVectorArray("u_light_position", property_position);

        mat.SetFloatArray("u_light_light_on", property_lighton);
        mat.SetFloatArray("u_light_slit_count", property_slit_count);
        mat.SetVectorArray("u_light_ambient_color", property_ambient);
        mat.SetVectorArray("u_light_diffuse_color", property_diffuse);
        mat.SetVectorArray("u_light_specular_color", property_specular);

        //for spot light
        mat.SetVectorArray("u_light_spot_direction", property_spot_dir);
        mat.SetFloatArray("u_light_spot_exponent", property_spot_exponent);
        mat.SetFloatArray("u_light_spot_cutoff_angle", property_cutoff_angle);

        mat.SetFloat("norm_vec_x", norm_exotic[0]);
        mat.SetFloat("norm_vec_y", norm_exotic[1]);
        mat.SetFloat("norm_vec_z", norm_exotic[2]);
    }

    //씬에 사용되는 빛을 설정.
    void set_up_scene_lights()
    {
        //파라미터로 저장
        //중앙의 빛. 전체적으로 넓은 범위를 비추고 있다.
        // point_light_EC: use light 0
        light_property[0].Pos_Type = COORD_EC;
        light_property[0].Light_Type = Light_Point;
        light_property[0].light_on = 1;
        light_property[0].slit_count = 0;

        light_property[0].position[0] = 0.0f; light_property[0].position[1] = 10.0f;  // point light position in EC
        light_property[0].position[2] = 0.0f; light_property[0].position[3] = 1.0f;

        light_property[0].ambient_color[0] = 0.3f; light_property[0].ambient_color[1] = 0.3f;
        light_property[0].ambient_color[2] = 0.3f; light_property[0].ambient_color[3] = 1.0f;

        light_property[0].diffuse_color[0] = 0.7f; light_property[0].diffuse_color[1] = 0.7f;
        light_property[0].diffuse_color[2] = 0.7f; light_property[0].diffuse_color[3] = 1.0f;

        light_property[0].specular_color[0] = 0.9f; light_property[0].specular_color[1] = 0.9f;
        light_property[0].specular_color[2] = 0.9f; light_property[0].specular_color[3] = 1.0f;

        //뒤쪽의 스포트 라이트
        // spot_light_WC: use light 1
        light_property[1].Pos_Type = COORD_WC;
        light_property[1].Light_Type = Light_Spot;
        light_property[1].light_on = 1;
        light_property[1].slit_count = 0;

        light_property[1].position[0] = -200.0f; light_property[1].position[1] = 500.0f; // spot light position in WC
        light_property[1].position[2] = -200.0f; light_property[1].position[3] = 1.0f;

        light_property[1].ambient_color[0] = 0.2f; light_property[1].ambient_color[1] = 0.2f;
        light_property[1].ambient_color[2] = 0.2f; light_property[1].ambient_color[3] = 1.0f;

        light_property[1].diffuse_color[0] = 0.82f; light_property[1].diffuse_color[1] = 0.82f;
        light_property[1].diffuse_color[2] = 0.82f; light_property[1].diffuse_color[3] = 1.0f;

        light_property[1].specular_color[0] = 0.82f; light_property[1].specular_color[1] = 0.82f;
        light_property[1].specular_color[2] = 0.82f; light_property[1].specular_color[3] = 1.0f;

        light_property[1].spot_direction[0] = 0.0f; light_property[1].spot_direction[1] = -1.0f; // spot light direction in WC
        light_property[1].spot_direction[2] = 0.0f;
        
        light_property[1].spot_cutoff_angle = 50.0f;//스포트 라이트 각도 20도
        light_property[1].spot_exponent = 27.0f;

        //light2
        light_property[2].Pos_Type = COORD_WC;
        light_property[2].Light_Type = Light_Spot;
        light_property[2].light_on = 0;
        light_property[2].slit_count = 0;

        light_property[2].position[0] = 200.0f; light_property[2].position[1] = 200.0f;
        light_property[2].position[2] = 200.0f; light_property[2].position[3] = 1.0f;

        light_property[2].ambient_color[0] = 0.2f; light_property[2].ambient_color[1] = 0.2f;
        light_property[2].ambient_color[2] = 0.2f; light_property[2].ambient_color[3] = 1.0f;

        light_property[2].diffuse_color[0] = 0.431f; light_property[2].diffuse_color[1] = 1.0f;
        light_property[2].diffuse_color[2] = 1.0f; light_property[2].diffuse_color[3] = 1.0f;

        light_property[2].specular_color[0] = 0.431f; light_property[2].specular_color[1] = 1.0f;
        light_property[2].specular_color[2] = 1.0f; light_property[2].specular_color[3] = 1.0f;

        light_property[2].spot_direction[0] = -1.0f; light_property[2].spot_direction[1] = -1.0f; 
        light_property[2].spot_direction[2] = -1.0f;

        light_property[2].spot_cutoff_angle = 20.0f;
        light_property[2].spot_exponent = 15.0f;
        //light3

        light_property[3].Pos_Type = COORD_WC;
        light_property[3].Light_Type = Light_Spot;
        light_property[3].position_default = new Vector4(0.0f, 0.0f, -50.0f, 1.0f);
        light_property[3].light_on = 0;
        light_property[3].slit_count = 0;

        light_property[3].position[0] = 0.0f; light_property[3].position[1] = 50.0f;
        light_property[3].position[2] = -80.0f; light_property[3].position[3] = 1.0f;

        light_property[3].ambient_color[0] = 0.2f; light_property[3].ambient_color[1] = 0.2f;
        light_property[3].ambient_color[2] = 0.2f; light_property[3].ambient_color[3] = 1.0f;

        light_property[3].diffuse_color[0] = 1.0f; light_property[3].diffuse_color[1] = 0.949f;
        light_property[3].diffuse_color[2] = 0.431f; light_property[3].diffuse_color[3] = 1.0f;

        light_property[3].specular_color[0] = 1.0f; light_property[3].specular_color[1] = 0.82f;
        light_property[3].specular_color[2] = 0.431f; light_property[3].specular_color[3] = 1.0f;

        light_property[3].spot_direction[0] = 0.0f; light_property[3].spot_direction[1] = 0.0f;
        light_property[3].spot_direction[2] = -1.0f;

        light_property[3].spot_cutoff_angle = 30.0f;//스포트 라이트 각도 20도
        light_property[3].spot_exponent = 20.0f;

        // need to supply position in EC for shading
        Camera cam = GetComponent<Camera>();//카메라가 중점을 바라보도록
        cam.transform.LookAt(Vector3.zero);

        set_light(material_ps_default);
    }

    //중심축의 material을 설정.
    void set_material_axes()
    {
        GameObject Axes = GameObject.Find("Axes");
        GameObject axes_x = Axes.transform.Find("Axes-x").gameObject;
        GameObject axes_y = Axes.transform.Find("Axes-y").gameObject;
        GameObject axes_z = Axes.transform.Find("Axes-z").gameObject;

        //세 축이 다른 색상(쉐이더의 변수가 다름)을 사용하므로 다른 메테리얼을 사용해야 한다.
        Renderer axes_x_renderer = axes_x.GetComponent<Renderer>();
        Material material1 = new Material(Shader.Find("HLSL/Simple_cg"));
        material1.SetColor("_Primitive_color", new Color(1, 0, 0, 0));
        axes_x_renderer.material = material1;

        Renderer axes_y_renderer = axes_y.GetComponent<Renderer>();
        Material material2 = new Material(Shader.Find("HLSL/Simple_cg"));
        material2.SetColor("_Primitive_color", new Color(0, 1, 0, 0));
        axes_y_renderer.material = material2;

        Renderer axes_z_renderer = axes_z.GetComponent<Renderer>();
        Material material3 = new Material(Shader.Find("HLSL/Simple_cg"));
        material3.SetColor("_Primitive_color", new Color(0, 0, 1, 0));
        axes_z_renderer.material = material3;

    }

    List<PlayObject> playObjectList;
    //광원 정보를 초기화하고 오브젝트들을 화면에 배치한다.
    void prepare_scene()
    {
        set_up_scene_lights();
        set_material_axes();
        playObjectList = new List<PlayObject>();
        //change!
        floor = new Floor(material_ps_default);
        screen = new Screen(material_ps_default);
        tiger = new Tiger(material_ps_default);
        ben = new Ben(material_ps_default);
        wolf = new Wolf(material_ps_default);
        spider = new Spider(material_ps_default);

        playObjectList.Add(floor);
        playObjectList.Add(tiger);
        playObjectList.Add(screen);
        playObjectList.Add(ben);
        playObjectList.Add(wolf);
        playObjectList.Add(spider);
    }

    //초기화 수행.
    void initialize_renderer()
    {
        //카메라가 원점을 향하도록 돌린다.
        Camera cam = GetComponent<Camera>();
        cam.farClipPlane = 3000;
        cam.transform.LookAt(Vector3.zero);
        initialize_lights_and_material();

        prepare_scene();
        move_cameraLight();
    }

    Vector3 cam_direction = new Vector3(1,1,1);
    float move_direction = -1.0f;

    //카메라 이동. 점점 멀어지다가 일정 이상 멀어지면 다가간다.
    public void move_camera()
    {
        /* TO DO : Move Camera 구현 */
        float speed = 40.0f;
        Camera cam = GetComponent<Camera>();
        if (cam.transform.position.x >= 1000.0f || cam.transform.position.x <= -1000.0f )
        {
            move_direction = 1.0f;
        }
        else if(cam.transform.position.x <= 80.0f && cam.transform.position.x >= -80.0f )
        {
            move_direction = -1.0f;
        }
        cam.transform.Translate(move_direction * Vector3.forward * speed);
        move_cameraLight();
    }
    //카메라 회전. 네 방향을 회전한다.
    public void rotate_camera()
    {
        /* TO DO : Move Camera 구현 */
        Camera cam = GetComponent<Camera>();
        cam.transform.RotateAround(Vector3.zero, Vector3.up, 90.0f);
        move_cameraLight();
    }

    public void move_cameraLight()
    {
        /* TO DO : Light 2 움직임 구현 */
        Vector3 direction;
        Camera cam = GetComponent<Camera>();

        light_property[2].position[0] = cam.transform.position[0];
        light_property[2].position[1] = cam.transform.position[1];
        light_property[2].position[2] = cam.transform.position[2];

        direction = cam.transform.forward;
        direction.Normalize();

        light_property[2].spot_direction[0] = direction[0];
        light_property[2].spot_direction[1] = direction[1];
        light_property[2].spot_direction[2] = direction[2];

    }

    bool init = false;
    void Start()
    {
        //방향축 2개 배치하고 호랑이, 바닥판 생성해야 함.
        initialize_renderer();

        initialize_button();
        init = true;
    }
    enum ButtonID  { Light0=1, Light1,Light2, Light3, Exotic, Blind, Screen, Screen_Effect, Frep_plus, Freq_minus, Jump, Back, Move_cam, Rotate_Cam, Stop_animation, Wolf, Spider, More1, More2 };

    //버튼 초기화
    void initialize_button()
    {
        GameObject btn = GameObject.Find("Button_light0");
        btn.GetComponentInChildren<Text>().text = "Light 0 toggle";
        Button b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Light0); });

        btn = GameObject.Find("Button_light1");
        btn.GetComponentInChildren<Text>().text = "Light 1 toggle";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Light1); });

        btn = GameObject.Find("Button_light2");
        btn.GetComponentInChildren<Text>().text = "Light 2 toggle";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Light2); });

        btn = GameObject.Find("Button_light3");
        btn.GetComponentInChildren<Text>().text = "Light 3 toggle";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Light3); });


        btn = GameObject.Find("Button_blind");
        btn.GetComponentInChildren<Text>().text = "Blind";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Blind); });

        btn = GameObject.Find("Button_Exotic");
        btn.GetComponentInChildren<Text>().text = "Exotic";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Exotic); });

        btn = GameObject.Find("Button_screen");
        btn.GetComponentInChildren<Text>().text = "Screen";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Screen); });

        btn = GameObject.Find("Button_screen_effect");
        btn.GetComponentInChildren<Text>().text = "Screen Effect";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Screen_Effect); });

        btn = GameObject.Find("Button_screen_freqency_up");
        btn.GetComponentInChildren<Text>().text = "Screen freq ++";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Frep_plus); });

        btn = GameObject.Find("Button_screen_freqency_down");
        btn.GetComponentInChildren<Text>().text = "Screen freq --";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Freq_minus); });

        btn = GameObject.Find("Button_ben_jump");
        btn.GetComponentInChildren<Text>().text = "Ben Jump";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Jump); });

        btn = GameObject.Find("Button_ben_back");
        btn.GetComponentInChildren<Text>().text = "Ben Back";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Back); });

        btn = GameObject.Find("Button_move_cam");
        btn.GetComponentInChildren<Text>().text = "Move Camera";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Move_cam); });

        btn = GameObject.Find("Button_rotate_cam");
        btn.GetComponentInChildren<Text>().text = "Rotate Camera";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Rotate_Cam); });

        btn = GameObject.Find("Button_stop");
        btn.GetComponentInChildren<Text>().text = "Stop Animation";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Stop_animation); });

        btn = GameObject.Find("Button_wolf");
        btn.GetComponentInChildren<Text>().text = "Wolf";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Wolf); });

        btn = GameObject.Find("Button_spider");
        btn.GetComponentInChildren<Text>().text = "Spider";
        b = btn.GetComponent<Button>();
        b.onClick.AddListener(delegate () { this.ButtonClicked((int)ButtonID.Spider); });
    }

    //버튼 이벤트 처리
    public void ButtonClicked(int btnId)
    {
        int lightID = -1;
        switch (btnId)
        {
            case (int)ButtonID.Light0:
                lightID = 0;
                break;
            case (int)ButtonID.Light1:
                lightID = 1;
                break;
            case (int)ButtonID.Light2:
                lightID = 2;
                break;
            case (int)ButtonID.Light3:
                lightID = 3;
                break;
            case (int)ButtonID.Blind:
                /* TO DO : Blind 효과 작성 */
                break;
            case (int)ButtonID.Exotic:
                exotic_flag = 1 - exotic_flag;
                if(exotic_flag == 0)
                    light_property[1].slit_count = 0;
                break;

            case (int)ButtonID.Screen:
                screen.changeScreenDraw();
                break;
            case (int)ButtonID.Screen_Effect:
                screen.changeScreenEffect();
                break;               
            case (int)ButtonID.Frep_plus:
                screen.freq_up();
                break;
            case (int)ButtonID.Freq_minus:
                screen.freq_down();
                break;
            case (int)ButtonID.Jump:
                ben.jump();
                break;

            case (int)ButtonID.Back:
                ben.back();
                break;

            case (int)ButtonID.Move_cam:
                move_camera();
                break;

            case (int)ButtonID.Rotate_Cam:
                rotate_camera();
                break;

            case (int)ButtonID.Stop_animation:
                animationFlag = 1 - animationFlag;
                break;

            case (int)ButtonID.Wolf:
                wolf.changeWolfDraw();
                break;

            case (int)ButtonID.Spider:
                spider.changeSpiderDraw();
                break;
        }

        user_event(lightID);
    }

    //버튼 이벤트 처리
    void user_event(int lightID)
    {
        if (lightID != -1)
        {
            light_property[lightID].light_on = 1 - light_property[lightID].light_on;

            float[] property = material_ps_default.GetFloatArray("u_light_light_on");
            property[lightID] = light_property[lightID].light_on;
            material_ps_default.SetFloatArray("u_light_light_on", property);

        }
        update_materials();
    }

    //쉐이더 변수(광원 등) 갱신, material 수정.
    void update_materials()
    {
        //유니티가 멈추면(창을 내리거나 하는 등 포커스를 잃으면) material의 property, uniform 데이터들이 전부 초기화된다..
        //그래서 매번 property를 다시 설정해 줘야만 한다.
      
        initialize_lights_and_material();
        set_light(material_ps_default);

        set_material_axes();

        for (int i = 0; i < playObjectList.Count; i++)
        {
            playObjectList[i].set_material(material_ps_default);
        }
    }

    // exotic 조명 효과: 여기서 slit_count를 늘린다. 2의 지수승으로 늘어난다.
    int exotic_flag = 0;
    int exotic_direction = 1;
    int mul = 2;
    int modulo = 7;
    float angle_exotic = 45.0f;
    void move_exotic()
    {
        /* TO DO : Exotic 효과 재작성 */

        if (exotic_flag == 1)
        {
            norm_exotic.x = 1.0f * Mathf.Cos(angle_exotic);
            norm_exotic.y = 0.0f;
            norm_exotic.z = 1.0f * Mathf.Sin(angle_exotic);
            angle_exotic = (angle_exotic + 1.0f) % 360.0f;

            if (exotic_direction == 1)
            {
                if (light_property[1].slit_count < 100000)
                {
                    light_property[1].slit_count *= (mul);
                    mul = (mul + 1 ) % modulo;
                    if(mul == 0)
                    {
                        mul += 7;
                    }
                    if (light_property[1].slit_count <= 1)
                    {
                        light_property[1].slit_count = 1;
                    }
                }
                else
                {
                    exotic_direction = -1;
                }
            }
            else
            {
                if (light_property[1].slit_count > 1)
                {
                    light_property[1].slit_count /= (mul);
                    mul = (mul - 1) % modulo;
                    if (mul == 0)
                    {
                        mul += 7;
                    }
                }
                else
                {
                    exotic_direction = 1;
                }
            }
            update_materials();
        }
    }

    // Frame rate 조절: 이 함수가 주기적으로 물체를 움직이는데 사용
    private bool delayCheck = true;
    IEnumerator WaitForIt()
    {
        yield return new WaitForSeconds(0.05f);
        delayCheck = true;
        drawFrame();
    }

    // Update is called once per frame
    void Update()
    {
        if (delayCheck)
        {
            delayCheck = false;
            StartCoroutine(WaitForIt());
        }
    }

    void drawFrame()
    {
        //이동
        if (init)
        {
            if (animationFlag == 1)
            {
                for (int i = 0; i < playObjectList.Count; i++)
                {
                    playObjectList[i].move();
                }
                move_exotic();
            }
        }
        update_materials();

        //모바일 핀치 줌(손가락으로 확대)
        if (Input.touchCount == 2)
        {
            Touch touchZero = Input.GetTouch(0); //첫번째 손가락 좌표
            Touch touchOne = Input.GetTouch(1); //두번째 손가락 좌표

            // deltaposition은 deltatime과 동일하게 delta만큼 시간동안 움직인 거리를 말한다.

            // 현재 position에서 이전 delta값을 빼주면 움직이기 전의 손가락 위치가 된다.
            Vector2 touchZeroPrevPos = touchZero.position - touchZero.deltaPosition;
            Vector2 touchOnePrevPos = touchOne.position - touchOne.deltaPosition;

            // 현재와 과거값의 움직임의 크기를 구한다.
            float prevTouchDeltaMag = (touchZeroPrevPos - touchOnePrevPos).magnitude;
            float touchDeltaMag = (touchZero.position - touchOne.position).magnitude;

            // 두 값의 차이는 즉 확대/축소할때 얼만큼 많이 확대/축소가 진행되어야 하는지를 결정한다.
            float deltaMagnitudeDiff = prevTouchDeltaMag - touchDeltaMag;

            Camera camera = GetComponent<Camera>();

            camera.transform.Translate(new Vector3(deltaMagnitudeDiff * 5, deltaMagnitudeDiff * 5, deltaMagnitudeDiff * 5), Space.World);

            if (camera.transform.position.x < 100)
            {
                camera.transform.position = new Vector3(100, 100, 100);
            }
            else if (camera.transform.position.x > 700)
            {
                camera.transform.position = new Vector3(700, 700, 700);
            }

            update_materials();
        }

    }

    private void OnApplicationFocus(bool focus)
    {
        //focus를 잃으면 데이터가 사라진다.
        if (init)
            update_materials();
    }
}
