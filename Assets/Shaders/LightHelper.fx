struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{ 
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

// 计算方向光
void ComputeDirectionalLight(Material mat, DirectionalLight L, 
                             float3 normal, float3 toEye,
					         out float4 ambient,
						     out float4 diffuse,
						     out float4 spec)
{
    // 初始化
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // 环境光
    ambient = L.Ambient * mat.Ambient;

    // 漫反射因子
    float3 lightVec = -L.Direction; // 光的反向
    float diffuseFactor = dot(lightVec, normal);
    
    if(diffuseFactor > 0.0f)
    {
        float3 lightReflect = reflect(L.Direction, normal);
        float specFactor = pow(max(dot(lightReflect, toEye), 0), mat.Specular.w);
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        if(specFactor > 0)
            spec = specFactor * mat.Specular * L.Specular;
    }
}

// 计算点光源
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
                    out float4 ambient, out float4 diffuse, out float4 spec)
{
    // 初始化
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // 漫反射因子
    float3 lightVec = L.Position - pos; // 从物体表面到灯
    float distance = length(lightVec);  // 眼到物体的距离
    
    // 超过距离就看不到了
    if(distance > L.Range)
    {
        return;
    }
    lightVec /= distance;

    // 环境光
    ambient = L.Ambient * mat.Ambient;


    float diffuseFactor = dot(lightVec, normal);
    if(diffuseFactor > 0.0f)
    {
        float3 lightReflect = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(lightReflect, toEye), 0), mat.Specular.w);
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        if(specFactor > 0)
            spec = specFactor * mat.Specular * L.Specular;
    }

    // 衰减
    float att = 1.0f / dot(L.Att, float3(1.0f, distance, distance * distance));
    diffuse *= att;
    spec *= att;
}

// 计算聚光灯
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
				  out float4 ambient, out float4 diffuse, out float4 spec)
{
    // 初始化
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // 漫反射因子
    float3 lightVec = L.Position - pos; // 从物体表面到灯
    float distance = length(lightVec);  // 眼到物体的距离
    // 超过距离就看不到了
    if(distance > L.Range)
    {
        return;
    }
    lightVec /= distance;

    // 环境光
    ambient = L.Ambient * mat.Ambient;

    float diffuseFactor = dot(lightVec, normal);
    if(diffuseFactor > 0.0f)
    {
        float3 lightReflect = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(lightReflect, toEye), 0.0f), mat.Specular.w);
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        if(specFactor > 0)
            spec = specFactor * mat.Specular * L.Specular;
    }

    // 汇聚强度
    float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

    // 衰减
    float att = spot / dot(L.Att, float3(1.0f, distance, distance * distance));

    ambient *= spot;
    diffuse *= att;
	spec *= att;
}