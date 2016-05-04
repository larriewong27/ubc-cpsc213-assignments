int a;
int b;
int c;
int val;

int foo(int a, int b, int c) {
	int tmp;
	switch (a) {
	case 10:
		tmp = b + c;
		break;
	case 12:
		tmp = b - c;
		break;
	case 14:
		if (b > c) {
			tmp = 1;
		}
		else tmp = 0;
		break;
	case 16:
		if (b < c) {
			tmp = 1;
		}
		else tmp = 0;
		break;
	case 18:
		if (b == c) {
			tmp = 1;
		}
		else tmp = 0;
		break;
	default:
		tmp = 0;
		break;
	}
	return tmp;
}

int main(void) {
	val = foo(a,b,c);
}