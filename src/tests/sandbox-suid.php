<?php
//echo "Testing...</br>";
include "octetos-db.php";
include "muposys.php";


$datconn = new octetos\Datconnect();
$datconn->create("localhost","muposys","123456","muposys-0.1-alpha",3306);
$conn = new octetos\Connector();

$conn->connect($datconn);


$number = rand(0,10000);
$userName = "user-" . $number;
$pername = "name-" . $number;
$newuser = new Users();
if($newuser->insert($conn,$userName,$pername))
{
	echo "inserted 1 $userName<br>";
}
else
{
	echo "Fallo insert <br>";
}
$strmd5 = date('Ymdhis');
if($newuser->getPerson()->getEnte()->upMd5sum($conn,$strmd5))
{
        echo "update id=" . $newuser->getPerson()->getEnte()->getID() . ", md5=" . $strmd5 . "<br>";
}

$number2 = rand(0,10000);
$userName2 = "user-" . $number2;
$pername2 = "name-" . $number2;

$newuser2 = new Users();
if($newuser2->insert($conn,$userName2,$pername2))
{
	echo "inserted 2 $userName2<br>";
}
else
{
	echo "Fallo insert<br>";
}


$number3 = rand(0,10000);
$userName3 = "user-" . $number3;
$pername3 = "name-" . $number3;

$newuser3 = new Users();
if($newuser3->insert($conn,$userName3,$pername3))
{
	echo "inserted 3 $userName3<br>";
}
else
{
	echo "Fallo insert <br>";
}
if($newuser3->remove($conn))
{
	echo "removed $userName3<br>";
}
else
{
	echo "Fallo remove <br>";
}
echo "<br>";

$userList = Users::select($conn," person != 0",10);
foreach ($userList as &$u) 
{
	$u->downName($conn);
	echo "id : " .  $u->getPerson()->getEnte()->getID() . " - " . $u->getName() . "<br>";
}

?>
