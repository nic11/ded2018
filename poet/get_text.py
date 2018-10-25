import requests

def parse_line(line):
    if not line.startswith('<A NAME=') or not line.endswith('</A><br>'):
        return ''
    return line[line.find('>') + 1 : -len('</A><br>')].strip()

r = requests.get('http://shakespeare.mit.edu/romeo_juliet/full.html')

lines = r.text.split('\n')
lines = map(parse_line, lines)
lines = filter(None, lines)
lines = list(lines)

open('romeo.txt', 'w').write('\n'.join(lines) + '\n')
