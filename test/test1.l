def f1 = fn (i1, i2: i32 = 12) : i32 {
  if i1 > 100 {
    //return i1 + (i1 * i2)
    return i1 + i2
  }
  else if i1 > -200 {
    return i1 * i2
  }
  else {
    //return i1 - (i1 * i2)
    return i1 - i2
  }
}

var v1 = 123, v2 : i32 = 456
v2 = 1+2*4-5

var v3 = f1(v1, v2)


/*
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
/*
v1 = +123.456
v2 = -456
*/
print("hallo")

func f1(a b:i64) {
  v1 = add(a b)
  v2 = add(v1 v1)

  add(v1 v2)
}

//v3 = f1(v1 v2)
print(v3)
/*
def c: const i32 = 12
def a = 1 @(if c > 0 { @+ } else { @- }) 2
*/

*/
