#version 330 core
layout(location = 0) in vec3 a_Position;    // ���_�̈ʒu
layout(location = 1) in vec2 a_TexCoord;    // UV���W
layout(location = 2) in vec3 a_Normal;      // ���_�@��

// out�F�O����
out vec2 v_TexCoord;    // UV���W
out vec3 v_Normal;      // �@���x�N�g��
out vec3 v_LightDir;    // ���̕���
out vec3 v_ViewDir;     // ���_�̕���

// uniform�F�O������
uniform mat4 u_Model;       // ���f���s��
uniform mat4 u_View;        // �r���[�s��
uniform mat4 u_Projection;  // �v���W�F�N�V�����s��
uniform vec3 u_LightPos;    // �����̈ʒu
uniform vec3 u_CameraPos;   // �J�����̈ʒu

void main() {
    // ���[���h���W�ɕϊ�
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);

    // �@�������[���h��Ԃɕϊ�
    // ���f���s��̋t�]�u�s����g���ĕό`
    vec3 worldNormal = mat3(transpose(inverse(u_Model))) * a_Normal;
    
    // �������Ǝ����������v�Z
    v_LightDir = normalize(u_LightPos - worldPos.xyz);
    v_ViewDir = normalize(u_CameraPos - worldPos.xyz);

    // �o�͏��
    v_TexCoord = a_TexCoord;
    v_Normal = normalize(worldNormal);

    // �ŏI�I�ȉ�ʍ��W�ɕϊ�
    gl_Position = u_Projection * u_View * worldPos;
}