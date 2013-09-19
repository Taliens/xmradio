#include <ctime>
#include <cstdio>
#include <cstdarg>
#include <cstdio>

#include "xmediaplayer_vlc.h"

XMR_USE_NS
using namespace std;

#ifdef X_OS_WIN

#include <Windows.h>

#define snprintf _snprintf

static void sleep(int secs)
{
	Sleep(secs * 1000);
}

#else

#include <unistd.h>

#endif // X_OS_WIN

static string cur_time()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	char sz[20] = { 0 };
	snprintf(sz, 20, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

	return sz;
}

static void print_info(const char *fmt, ...)
{
	va_list args;
	
	printf("[%s] ", cur_time().c_str());

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

static string make_time_str(long long time)
{
	long hours = time / 1000 / 60 / 60 % 24;
	long mins = time / 1000 / 60 % 60;
	long secs = time / 1000 % 60;
	long ms = time % 1000;
	
	char str[50] = { 0 };
	sprintf(str, "%02ld:%02ld:%02ld.%03ld", hours, mins, secs, ms);
	
	return str;
}

static string make_state_str(XMediaPlayer::State state)
{
	switch (state)
	{
	case XMediaPlayer::Nothing:
		return "Nothing";
		
	case XMediaPlayer::Opening:
		return "Opening";

	case XMediaPlayer::Buffering:
		return "Buffering";

	case XMediaPlayer::Playing:
		return "Playing";

	case XMediaPlayer::Paused:
		return "Paused";

	case XMediaPlayer::Stopped:
		return "Stopped";
	}
	
	return "Unexpected";
}

static void player_event(XMediaPlayer::Event type, XMediaPlayer *player)
{
	switch(type)
	{
	case XMediaPlayer::PlayerError:
		print_info("Player encountered error: %s\n", player->errorString().c_str());
		break;

	case XMediaPlayer::PlayerMediaChanged:
		print_info("Media changed\n");
		break;
		
	case XMediaPlayer::PlayerBuffering:
		print_info("Buffering (%.2f%%)\n", player->buffering());
		break;
		
	case XMediaPlayer::PlayerStateChanged:
		print_info("Player state changed (%s)\n", make_state_str(player->state()).c_str());
		break;

	case XMediaPlayer::PlayerEndReadched:
		print_info("End readched\n");
		break;
		
	case XMediaPlayer::PlayerTimeChanged:
		print_info("Time changed (%s/%s)\n",
				   make_time_str(player->getTime()).c_str(),
				   make_time_str(player->duration()).c_str());
		break;
		
	default:
		break;
	}
}

int main(int argc, char **argv)
{
	XMediaPlayer *player = new XMediaPlayerVlc;
	
	player->setEventCallback((XMediaPlayer::callback_t)player_event, player);

	player->watch(XMediaPlayer::PlayerError);
	player->watch(XMediaPlayer::PlayerMediaChanged);
	player->watch(XMediaPlayer::PlayerBuffering);
	player->watch(XMediaPlayer::PlayerStateChanged);
	player->watch(XMediaPlayer::PlayerEndReadched);
	player->watch(XMediaPlayer::PlayerTimeChanged);

	print_info("Test begin with backend: %s\n", player->name());
	player->open("http://m1.file.xiami.com/1/2/35002/214258/2535804_169622_l.mp3");

	print_info("Should play 10 seconds\n");
	sleep(10);

	print_info("Should pause music 5 seconds\n");
	player->pause();
	sleep(5);
	
	print_info("Unwatch PlayerTimeChanged event\n");
	player->unwatch(XMediaPlayer::PlayerTimeChanged);

	print_info("OK, let's resume for another 5 seconds\n");
	player->resume();
	sleep(5);

	print_info("Now open another stream and play after 3 seconds\n");
	player->open("http://m1.file.xiami.com/836/1836/4279/1771128942_3578913_l.mp3", false);
	sleep(3);

	player->play();
	print_info("Play for 5 seconds\n");
	sleep(5);
	
	print_info("Now stop...\n");
	player->stop();
	
	print_info("Try to play the stopped media\n");
	if (player->play())
		print_info("Should play from the beginning\n");
	else
		print_info("Can't play after stop!!!\n");

	print_info("isMuted: %d\n", player->isMuted() ? 1 : 0);
	print_info("volume: %d\n", player->getVolume());
	printf("setVoume(80)\n");
	player->setVolume(80);
	print_info("new volume: %d\n", player->getVolume());

	print_info("Sleep for 5 seconds\n");
	sleep(5);
	
	print_info("setMuted for 3s\n");
	player->setMuted(true);
	sleep(3);
	player->setMuted(false);
	
	print_info("setLoopCount(2)\n");
	player->setLoopCount(2);

	print_info("Press any key to exit\n");
	getchar();

	delete player;

	print_info("Test done, now exit\n");

	return 0;
}
