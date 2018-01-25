import json
from os.path import join, dirname
from watson_developer_cloud import ToneAnalyzerV3

tone_analyzer = ToneAnalyzerV3(
    username='c032fda0-5c02-490d-8e00-ab00de2e5f40',
    password='AfgP2LQIDCgB',
    version='2017-09-26')

print("\ntone_chat() example 1:\n")
utterances = [{'text': 'Today, I didn\'t get to do anything that I wanted to. W'},
              {'text': 'It is a good day.'},
              {'text': 'The world is a very weird place. So many interesting and different people.'}]
print(json.dumps(tone_analyzer.tone_chat(utterances), indent=2))

print("\ntone() example 2:\n")
with open(join(dirname(__file__),
               '../resources/text.json')) as tone_json:
    tone = tone_analyzer.tone(json.load(tone_json)['text'], "text/plain")

file = open('../resources/tone_data.txt', 'w')
file.write(json.dumps(tone, indent=2))