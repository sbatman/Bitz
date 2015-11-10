#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <android/sensor.h>

#include <android/log.h>
#include "android_native_app_glue.h"

#include "Core.h"
#include "GameLogic\GameCore.h"
#include "Graphics\GraphicsManager.h"
#include "Graphics\Window.h"
#include "Math\Math.h"
#include "Debug\Logging.h"

#include <EGL/egl.h>
#include <GLES/gl.h>
/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * This file is to be considered heavily modified from the above licensed origional
 * SBatchelor-Manning 2015
 *
 */

Bitz::GameLogic::GameCore *_GameInstance = nullptr;
Bitz::PlatformSpecific::Android::GFX::Window * _Window = nullptr;
AAssetManager * AndroidAssetManager = nullptr;

static void free_saved_state(struct android_app* android_app)
{
	pthread_mutex_lock(&android_app->mutex);
	if (android_app->savedState != NULL) {
		free(android_app->savedState);
		android_app->savedState = NULL;
		android_app->savedStateSize = 0;
	}
	pthread_mutex_unlock(&android_app->mutex);
}

AAssetManager * android_app_GetAssetManager()
{
	return AndroidAssetManager;
}

int8_t android_app_read_cmd(struct android_app* android_app)
{
	if (android_app == nullptr)return -1;
	int8_t cmd;
	if (read(android_app->msgread, &cmd, sizeof(cmd)) == sizeof(cmd))
	{
		switch (cmd) {
		case APP_CMD_SAVE_STATE:
			free_saved_state(android_app);
			break;
		}
		return cmd;
	}
	else {
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Error, "Android - No data on command pipe!");
	}
	return -1;
}

void android_app_pre_exec_cmd(struct android_app* android_app, int8_t cmd)
{
	switch (cmd) {
	case APP_CMD_INPUT_CHANGED:
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, "Android - APP_CMD_INPUT_CHANGED");
		pthread_mutex_lock(&android_app->mutex);
		if (android_app->inputQueue != NULL) {
			AInputQueue_detachLooper(android_app->inputQueue);
		}
		android_app->inputQueue = android_app->pendingInputQueue;
		if (android_app->inputQueue != NULL) {
			Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, "Android - Attaching input queue to looper");
			AInputQueue_attachLooper(android_app->inputQueue,
				android_app->looper, LOOPER_ID_INPUT, NULL,
				&android_app->inputPollSource);
		}
		pthread_cond_broadcast(&android_app->cond);
		pthread_mutex_unlock(&android_app->mutex);
		break;

	case APP_CMD_INIT_WINDOW:
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, "Android - APP_CMD_INIT_WINDOW\n");
		pthread_mutex_lock(&android_app->mutex);
		android_app->window = android_app->pendingWindow;
		pthread_cond_broadcast(&android_app->cond);
		pthread_mutex_unlock(&android_app->mutex);
		break;

	case APP_CMD_TERM_WINDOW:
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, "Android - APP_CMD_TERM_WINDOW\n");
		pthread_cond_broadcast(&android_app->cond);
		break;

	case APP_CMD_RESUME:
	case APP_CMD_START:
	case APP_CMD_PAUSE:
	case APP_CMD_STOP:
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - activityState={0}", cmd));
		pthread_mutex_lock(&android_app->mutex);
		android_app->activityState = cmd;
		pthread_cond_broadcast(&android_app->cond);
		pthread_mutex_unlock(&android_app->mutex);
		break;

	case APP_CMD_CONFIG_CHANGED:
	{
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, "Android - APP_CMD_CONFIG_CHANGED\n");
		AConfiguration_fromAssetManager(android_app->config, android_app->activity->assetManager);
	}
	break;

	case APP_CMD_DESTROY:
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, "Android - APP_CMD_DESTROY\n");
		android_app->destroyRequested = 1;
		break;
	}
}

void android_app_post_exec_cmd(struct android_app* android_app, int8_t cmd)
{
	switch (cmd) {
	case APP_CMD_TERM_WINDOW:
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, "Android - APP_CMD_TERM_WINDOW\n");
		pthread_mutex_lock(&android_app->mutex);
		android_app->window = NULL;
		pthread_cond_broadcast(&android_app->cond);
		pthread_mutex_unlock(&android_app->mutex);
		break;

	case APP_CMD_SAVE_STATE:
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, "Android - APP_CMD_SAVE_STATE\n");
		pthread_mutex_lock(&android_app->mutex);
		android_app->stateSaved = 1;
		pthread_cond_broadcast(&android_app->cond);
		pthread_mutex_unlock(&android_app->mutex);
		break;

	case APP_CMD_RESUME:
		free_saved_state(android_app);
		break;
	}
}

static void android_app_destroy(struct android_app* android_app)
{
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, "Android - android_app_destroy!");
	free_saved_state(android_app);
	pthread_mutex_lock(&android_app->mutex);
	if (android_app->inputQueue != NULL) {
		AInputQueue_detachLooper(android_app->inputQueue);
	}
	AConfiguration_delete(android_app->config);
	android_app->destroyed = 1;
	pthread_cond_broadcast(&android_app->cond);
	pthread_mutex_unlock(&android_app->mutex);
}

static void process_input(struct android_app* app, struct android_poll_source* source)
{
	AInputEvent* event = NULL;
	while (AInputQueue_getEvent(app->inputQueue, &event) >= 0) {
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - New input event: type={0}", AInputEvent_getType(event)));
		if (AInputQueue_preDispatchEvent(app->inputQueue, event)) {
			continue;
		}
		int32_t handled = 0;
		if (app->onInputEvent != NULL) handled = app->onInputEvent(app, event);
		AInputQueue_finishEvent(app->inputQueue, event, handled);
	}
}

static void process_cmd(struct android_app* app, struct android_poll_source* source)
{
	int8_t cmd = android_app_read_cmd(app);
	android_app_pre_exec_cmd(app, cmd);
	if (app->onAppCmd != NULL) app->onAppCmd(app, cmd);
	android_app_post_exec_cmd(app, cmd);
}

static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		app->stateSaved = true;
		break;

	case APP_CMD_INIT_WINDOW:
		if (_GameInstance != NULL&&_Window != nullptr)
		{
			Bitz::GFX::GraphicsManager::SetActiveWindow(nullptr);
			_Window = nullptr;
		}
		if (_GameInstance != NULL&&_Window == nullptr)
		{
			_Window = new Bitz::PlatformSpecific::Android::GFX::Window(app);
			Bitz::GFX::GraphicsManager::SetActiveWindow(_Window);
		}

		break;
	case APP_CMD_TERM_WINDOW:

		Bitz::Core::SetRenderPause(true);
		break;
	case APP_CMD_GAINED_FOCUS:
		Bitz::Core::SetRenderPause(false);
		break;
	case APP_CMD_LOST_FOCUS:
		Bitz::Core::SetRenderPause(true);
		break;

	case APP_CMD_DESTROY:
		app->destroyed = true;
		Bitz::Core::Stop();
		if (_GameInstance != NULL&&_Window != nullptr)
		{
			Bitz::GFX::GraphicsManager::SetActiveWindow(nullptr);
			_Window = nullptr;
		}
		break;
	}
}

static void* android_app_entry(void* param) {
	struct android_app* android_app = (struct android_app*)param;

	android_app->config = AConfiguration_new();
	AConfiguration_fromAssetManager(android_app->config, android_app->activity->assetManager);

	android_app->cmdPollSource.id = LOOPER_ID_MAIN;
	android_app->cmdPollSource.app = android_app;
	android_app->cmdPollSource.process = process_cmd;
	android_app->inputPollSource.id = LOOPER_ID_INPUT;
	android_app->inputPollSource.app = android_app;
	android_app->inputPollSource.process = process_input;

	ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
	ALooper_addFd(looper, android_app->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL, &android_app->cmdPollSource);
	android_app->looper = looper;

	pthread_mutex_lock(&android_app->mutex);
	android_app->running = 1;
	pthread_cond_broadcast(&android_app->cond);
	pthread_mutex_unlock(&android_app->mutex);

	android_app->onAppCmd = engine_handle_cmd;
	while (_Window == nullptr)
	{
		int ident;
		int events;
		struct android_poll_source* source;
		while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
		{
			if (source != NULL) source->process(android_app, source);
		}
	}
	AndroidAssetManager = android_app->activity->assetManager;
	_GameInstance->Run(_Window);

	delete _GameInstance;

	android_app_destroy(android_app);
	return NULL;
}

static struct android_app* android_app_create(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
	struct android_app* android_app = (struct android_app*)malloc(sizeof(struct android_app));
	memset(android_app, 0, sizeof(struct android_app));
	android_app->activity = activity;

	pthread_mutex_init(&android_app->mutex, NULL);
	pthread_cond_init(&android_app->cond, NULL);

	if (savedState != NULL) {
		android_app->savedState = malloc(savedStateSize);
		android_app->savedStateSize = savedStateSize;
		memcpy(android_app->savedState, savedState, savedStateSize);
	}

	int msgpipe[2];
	if (pipe(msgpipe)) {
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Error, fmt::format("Android - could not create pipe: {0}", strerror(errno)));
		return NULL;
	}
	android_app->msgread = msgpipe[0];
	android_app->msgwrite = msgpipe[1];

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&android_app->thread, &attr, android_app_entry, android_app);

	// Wait for thread to start.
	pthread_mutex_lock(&android_app->mutex);
	while (!android_app->running) {
		pthread_cond_wait(&android_app->cond, &android_app->mutex);
	}
	pthread_mutex_unlock(&android_app->mutex);

	return android_app;
}

static void android_app_write_cmd(struct android_app* android_app, int8_t cmd) {
	if (write(android_app->msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
		Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Error, fmt::format("Android - Failure writing android_app cmd: {0}", strerror(errno)));
	}
}

static void android_app_set_input(struct android_app* android_app, AInputQueue* inputQueue) {
	pthread_mutex_lock(&android_app->mutex);
	android_app->pendingInputQueue = inputQueue;
	android_app_write_cmd(android_app, APP_CMD_INPUT_CHANGED);
	while (android_app->inputQueue != android_app->pendingInputQueue) {
		pthread_cond_wait(&android_app->cond, &android_app->mutex);
	}
	pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_set_window(struct android_app* android_app, ANativeWindow* window) {
	pthread_mutex_lock(&android_app->mutex);
	if (android_app->pendingWindow != NULL) {
		android_app_write_cmd(android_app, APP_CMD_TERM_WINDOW);
	}
	android_app->pendingWindow = window;
	if (window != NULL) {
		android_app_write_cmd(android_app, APP_CMD_INIT_WINDOW);
	}
	while (android_app->window != android_app->pendingWindow) {
		pthread_cond_wait(&android_app->cond, &android_app->mutex);
	}
	pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_set_activity_state(struct android_app* android_app, int8_t cmd) {
	pthread_mutex_lock(&android_app->mutex);
	android_app_write_cmd(android_app, cmd);
	while (android_app->activityState != cmd) {
		pthread_cond_wait(&android_app->cond, &android_app->mutex);
	}
	pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_free(struct android_app* android_app) {
	pthread_mutex_lock(&android_app->mutex);
	android_app_write_cmd(android_app, APP_CMD_DESTROY);
	while (!android_app->destroyed) {
		pthread_cond_wait(&android_app->cond, &android_app->mutex);
	}
	pthread_mutex_unlock(&android_app->mutex);

	close(android_app->msgread);
	close(android_app->msgwrite);
	pthread_cond_destroy(&android_app->cond);
	pthread_mutex_destroy(&android_app->mutex);
	free(android_app);
}

static void onDestroy(ANativeActivity* activity) {
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - Destroy: {0}", reinterpret_cast<int>(activity)));
	android_app_free((struct android_app*)activity->instance);
}

static void onStart(ANativeActivity* activity) {
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - Start: {0}", reinterpret_cast<int>(activity)));
	android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_START);
}

static void onResume(ANativeActivity* activity) {
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - Resume: {0}", reinterpret_cast<int>(activity)));
	android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_RESUME);
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen) {
	struct android_app* android_app = (struct android_app*)activity->instance;
	void* savedState = NULL;

	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - SaveInstanceState: {0}", reinterpret_cast<int>(activity)));
	pthread_mutex_lock(&android_app->mutex);
	android_app->stateSaved = 0;
	android_app_write_cmd(android_app, APP_CMD_SAVE_STATE);
	while (!android_app->stateSaved) {
		pthread_cond_wait(&android_app->cond, &android_app->mutex);
	}

	if (android_app->savedState != NULL) {
		savedState = android_app->savedState;
		*outLen = android_app->savedStateSize;
		android_app->savedState = NULL;
		android_app->savedStateSize = 0;
	}

	pthread_mutex_unlock(&android_app->mutex);

	return savedState;
}

static void onPause(ANativeActivity* activity) {
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - Pause: {0}", reinterpret_cast<int>(activity)));
	android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_PAUSE);
}

static void onStop(ANativeActivity* activity) {
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - Stop: {0}", reinterpret_cast<int>(activity)));
	android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_STOP);
}

static void onConfigurationChanged(ANativeActivity* activity) {
	struct android_app* android_app = (struct android_app*)activity->instance;
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - ConfigurationChanged: {0}", reinterpret_cast<int>(activity)));
	android_app_write_cmd(android_app, APP_CMD_CONFIG_CHANGED);
}

static void onLowMemory(ANativeActivity* activity) {
	struct android_app* android_app = (struct android_app*)activity->instance;
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - LowMemory: {0}", reinterpret_cast<int>(activity)));
	android_app_write_cmd(android_app, APP_CMD_LOW_MEMORY);
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused) {
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - WindowFocusChanged: {0} -- {1}", reinterpret_cast<int>(activity), focused));
	android_app_write_cmd((struct android_app*)activity->instance,
		focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - NativeWindowCreated: {0} -- {1}", reinterpret_cast<int>(activity), reinterpret_cast<int>(window)));
	android_app_set_window((struct android_app*)activity->instance, window);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - NativeWindowDestroyed:  {0} -- {1}", reinterpret_cast<int>(activity), reinterpret_cast<int>(window)));
	android_app_set_window((struct android_app*)activity->instance, NULL);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - InputQueueCreated: {0} -- {1}", reinterpret_cast<int>(activity), reinterpret_cast<int>(queue)));
	android_app_set_input((struct android_app*)activity->instance, queue);
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - InputQueueDestroyed: {0} -- {1}", reinterpret_cast<int>(activity), reinterpret_cast<int>(queue)));
	android_app_set_input((struct android_app*)activity->instance, NULL);
}

void ANativeActivity_Create(ANativeActivity* activity, void* savedState, size_t savedStateSize, Bitz::GameLogic::GameCore *gameInstance)
{
	Bitz::Debug::Logging::Log(Bitz::Debug::Logging::ErrorType::Notice, fmt::format("Android - Creating: (0)", reinterpret_cast<int>(activity)));
	_GameInstance = gameInstance;
	activity->callbacks->onDestroy = onDestroy;
	activity->callbacks->onStart = onStart;
	activity->callbacks->onResume = onResume;
	activity->callbacks->onSaveInstanceState = onSaveInstanceState;
	activity->callbacks->onPause = onPause;
	activity->callbacks->onStop = onStop;
	activity->callbacks->onConfigurationChanged = onConfigurationChanged;
	activity->callbacks->onLowMemory = onLowMemory;
	activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
	activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
	activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
	activity->callbacks->onInputQueueCreated = onInputQueueCreated;
	activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

	activity->instance = android_app_create(activity, savedState, savedStateSize);
}
