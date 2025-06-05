#version 330 core
layout(location = 0) in vec3 a_Position;    // 頂点の位置
layout(location = 1) in vec2 a_TexCoord;    // UV座標
layout(location = 2) in vec3 a_Normal;      // 頂点法線

// out：外部へ
out vec2 v_TexCoord;    // UV座標
out vec3 v_Normal;      // 法線ベクトル
out vec3 v_LightDir;    // 光の方向
out vec3 v_ViewDir;     // 視点の方向

// uniform：外部から
uniform mat4 u_Model;       // モデル行列
uniform mat4 u_View;        // ビュー行列
uniform mat4 u_Projection;  // プロジェクション行列
uniform vec3 u_CameraPos;   // カメラの位置

void main() {
    // ワールド座標に変換
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);

    // 法線をワールド空間に変換
    // モデル行列の逆転置行列を使って変形
    vec3 worldNormal = mat3(transpose(inverse(u_Model))) * a_Normal;
    
    // 光は常に真上から
    v_LightDir = normalize(vec3(0.0,1.0,0.0));

    // ビュー上の座標
    vec3 viewPos = vec3(u_View * worldPos);
    // ビュー上のカメラ座標
    vec3 cameraViewPos = vec3(u_View * vec4(u_CameraPos, 1.0));

    // 視点の方向
    v_ViewDir = normalize(cameraViewPos - viewPos);

    // 出力情報
    v_TexCoord = a_TexCoord;
    v_Normal = normalize(worldNormal);

    // 最終的な画面座標に変換
    gl_Position = u_Projection * u_View * worldPos;
}