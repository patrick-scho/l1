fn add(a b:i64):i64 (
  C (( a + b ))
)
fn print(str: string):void (
  C (( printf("%s\n", str) ))
)
fn print(i:i64):void (
  C (( printf("%d\n", i) ))
)

v1 = 123
v2 = 456

print("hallo")

fn f1(a b:i64) (
  v1 = add(a b)
  v2 = add(v1 v1)

  add(v1 v2)
)

v3 = f1(v1 v2)
print(v3)