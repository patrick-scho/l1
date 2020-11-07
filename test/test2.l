func add(i1 i2:i64) i64 (
  C (( i1 + i2 ))
)
func sub(i1 i2:i64) i64 (
  C (( i1 - i2 ))
)
func print(i:i64) void (
  C (( printf("%lld\n", i) ))
)
func print(s:string) void (
  C (( printf("%s\n", s) ))
)

meta m1(i:i32) (
  @print("hello")

  print("he\"n\"lo")

  print(@add(1 1))

  print(@i)
)

## 1.
result = ""

print("hello")

add(result "print(\"he\\\"n\\\"lo\")")

add(result "print(")

add(result add(1 1))

add(result ")")
##

## 2.
print("he\"n\"lo")
print(2)
##

m1()