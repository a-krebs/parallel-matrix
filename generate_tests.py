class Test(object):
	def __init__(self, p, s, *args, **kwargs):
		self.p = p
		self.s = s

def print_tests(exe, tests):
	for t in tests:
		for i in xrange(7):
			print "./{exe} -p {p} -s {s} >> output.txt".format(
				exe=exe, p=t.p, s=t.s)

def get_tests(p_options):
	tests = list()
	s = 2
	while s <= 2048:
		for p in p_options:
			# make sure test is valid
			if s % p != 0:
				continue
			tests.append(Test(p, s))
		s = getstep(s)
	return tests

def generate_tests():
	print "#!/bin/sh"
	parallel_tests = get_tests([1,2,4,8])
	sequential_tests = get_tests([1])
	print_tests('parallel', parallel_tests)
	print_tests('sequential', sequential_tests)
		

def getstep(val):
	if val < 16:
		return val + 2
	else:
		if val < 512:
			return val * 2;
		else:
			return val + 256;

if __name__ == "__main__":
	generate_tests()

