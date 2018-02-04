import json
from os.path import join, dirname
from watson_developer_cloud import ToneAnalyzerV3
import os


# Include .json in the filename
def analyzeText(input_filename, output_filename):
    tone_analyzer = ToneAnalyzerV3(
        version='2017-09-26',
        username='c032fda0-5c02-490d-8e00-ab00de2e5f40',
        password='AfgP2LQIDCgB'
    )

    with open(join(dirname(__file__), '../resources/' + input_filename)) as file_json:
        file_tone = tone_analyzer.tone(json.load(file_json)['text'], "text/plain")

    json_dump = open('../resources/json_dump.json', 'w')
    json_dump.write(json.dumps(file_tone, indent=2))
    json_dump.close()

    file_out = open('../resources/' + output_filename, 'w')
    with open('../resources/json_dump.json', 'r') as f:
        json_data = json.load(f)

    doc_tones = json_data['document_tone']
    file_out.write('[Document tones]\n')
    for tone in doc_tones['tones']:
        file_out.write(tone['tone_name'] + ':' + str(tone['score']) + '\n')

    sentence_tones = {}

    sen_tones = json_data['sentences_tone']
    for sentence in sen_tones:
        for tone in sentence['tones']:
            if tone['tone_name'] not in sentence_tones:
                sentence_tones[tone['tone_name']] = 1
            else:
                sentence_tones[tone['tone_name']] += 1

    file_out.write('[Sentence tone frequencies]\n')
    for tone, freq in sentence_tones.items():
        file_out.write(tone + ':' + str(freq) + '\n')

    os.remove('../resources/json_dump.json')
    file_out.close()
    f.close()
    file_json.close()


# Uncomment line below to test
# analyzeText('text.json', 'tone_data.txt')