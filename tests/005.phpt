--TEST--
test_scale() Basic test
--EXTENSIONS--
test
--FILE--
<?php
var_dump(test_scale(2));
var_dump(test_scale(2, 3));
var_dump(test_scale(2.0, 3));
var_dump(test_scale("2", 3));
var_dump(test_scale([2, 2.0, "x" => ["2"]], 3));

ini_set('test.scale', 5);
var_dump(test_scale(2));
?>
--EXPECT--
int(2)
int(6)
float(6)
string(3) "222"
array(3) {
  [0]=>
  int(6)
  [1]=>
  float(6)
  ["x"]=>
  array(1) {
    [0]=>
    string(3) "222"
  }
}
int(10)
