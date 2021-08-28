#include <jni.h>
#include <exception>
#include "LogHelper.h"

#include "graphics/Renderer.h"

const char* TAG = "NATIVE-LIB";

rend::Renderer* aRenderer = nullptr;

/*
extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_graphics_drawable_Triangle_triangleInit
	(JNIEnv *env, jobject thiz, jfloatArray jVertices)
{
	const size_t len = 9;

	jfloat *jVert = env->GetFloatArrayElements(jVertices, 0);
	float vertices[len];
	for (int i = 0; i < len; i++) {
		vertices[i] = jVert[i];
	}

	// Create VAO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(
			//Индекс вершинного аттрибута
			0,
			//Длина вершинного аттрибута в единицах
			3,
			//Тип данных
			GL_FLOAT,
			//Нужна ли нормализация
			GL_FALSE,
			//Длина шага в байтах
			3 * sizeof(GLfloat),
			//на сколько сдвинут вершинный аттрибут относительно начала
			(GLvoid *) (0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	jclass triangle = env->FindClass("ru/killthereal/illegal_racer/graphics/drawable/Triangle");
	jmethodID updateID = env->GetMethodID(triangle, "updateVAOandVBO", "(II)V");
	env->CallVoidMethod(thiz, updateID, (jint)VAO, (jint)VBO);
}
*/

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_jniCreateRenderer(
        JNIEnv *env,
        jobject thiz) {
    aRenderer = new rend::Renderer();
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_jniDestroyRenderer(
        JNIEnv *env,
        jobject thiz) {
    if (!aRenderer)
        LOGW(TAG, "Attempt to destroy an uninitialized renderer");
    else {
        delete aRenderer;
        aRenderer = nullptr;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_jniIsRendererAlive(JNIEnv *env, jobject thiz) {
    return aRenderer != nullptr;
}