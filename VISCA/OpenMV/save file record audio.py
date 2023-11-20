import image, audio, time

audio.init(channels=2, frequency=16000, gain_db=20, highpass=0.9883)

def audio_callback(buf):
    try:
        global fout
        fout.write(buf)
    except Exception as e:
        print("exption:", e)

# Open PCM file
fout = open('test.pcm', 'w')

# Start audio streaming
audio.start_streaming(audio_callback)

# do something else
print("recording audio...")
start = time.ticks_ms()
while ((time.ticks_ms() - start) < 5000):
    time.sleep_ms(100)

# Stop streaming
audio.stop_streaming()
fout.close()
