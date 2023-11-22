import urllib.request 
import subprocess
import requests

class Subtitles():
    def getSubtitles(self,input_file, output_file):
        
        subtitles_url = 'https://raw.githubusercontent.com/paudBatlle/UNI_work/6eeb37fbcfc08bfb1c074f35b9e62ef2df64abb5/Video%20Encoding/big_buck_bunny.eng.srt'
        subtitles_file = 'subtitles.srt'
        response = requests.get(subtitles_url)
        
        if response.status_code == 200:
            with open(subtitles_file, 'wb') as file:
                file.write(response.content)
        else:
            print(f"Failed to download subtitles. HTTP status code: {response.status_code}")
            return

        merge_subtitles_command = [
            'ffmpeg',
            '-i', input_file,
            '-vf', f'subtitles={subtitles_file}',
            '-c:a', 'copy',
            output_file
        ]

        subprocess.run(merge_subtitles_command)

