s = """EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCKQPKUGKMGOLICGINCGACKSNISACYKZSCKXECJCKSH
       YSXCGOIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZUGFZCCNDGYYSFUSZCNXEOJNCGYEOWEUPXEZG
       ACGNFGLKNSACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNCIACZEJNCSHFZEJZEGMXCYHCJUMGKUCY"""


def single_stat():
	for c in range(26):
		print(chr(c + ord('A')) + ' %.3f' % (s.count(chr(c + ord('A')))/len(s)))
	print()

def double_stat():
	stat = {}
	for i, c in enumerate(s):
		if i == len(s) - 2:
			break
		if s[i: i + 2] not in stat:
			stat[s[i: i + 2]] = 0
		stat[s[i: i + 2]] += 1
	count = sum(stat.values())
	for k in stat.keys():
		print(f'{k}: %.4f' % (float(stat[k]) / float(count) * 100), end='\t')
	print()

def sub():
	for c in s:
		if c == 'C':
			print('e', end='')
		elif c == 'G':
			print('a', end='')
		elif c == 'S':
			print('o', end='')
		elif c == 'F':
			print('w', end='')
		elif c == 'K':
			print('s', end='')
		elif c == 'I':
			print('d', end='')
		elif c == 'Z':
			print('h', end='')
		elif c == 'N':
			print('l', end='')
		elif c == 'U':
			print('t', end='')
		elif c == 'E':
			print('i', end='')
		elif c == 'Y':
			print('r', end='')
		elif c == 'M':
			print('m', end='')
		elif c == 'D':
			print('b', end='')
		elif c == 'L':
			print('y', end='')
		elif c == 'O':
			print('n', end='')
		elif c == 'W':
			print('g', end='')
		elif c == 'H':
			print('f', end='')
		elif c == 'P':
			print('u', end='')
		elif c == 'A':
			print('v', end='')
		elif c == 'X':
			print('p', end='')
		elif c == 'J':
			print('c', end='')
		elif c == ' ' or c == '\t' or c == '\n':
			continue
		else:
			print(c, end='')
	print()
	

if __name__ == '__main__':
	# double_stat()
	sub()
