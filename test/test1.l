type vec3 {
  x y z: f32
}

type a {
  i1: i16, i2: i32
  f1: f32, f2: f64
}

func add(a b:i64) i64 {
  C (( a + b ))
}
func print(str: string) void {
  C (( printf("%s\n", @str) ))
}
func print(i:i64) void {
  C (( printf("%d\n", @i) ))
}

v1 = +123.456
v2 = -456

print("hallo")

func f1(a b:i64) {
  v1 = add(a b)
  v2 = add(v1 v1)

  add(v1 v2)
}

//v3 = f1(v1 v2)
print(v3)
