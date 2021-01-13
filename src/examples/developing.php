<?php
//echo "Testing...</br>";
include "octetos-db.php";
include "muposys.php";


$datconn = new octetos\Datconnect();
$datconn->create("localhost","muposys","123456","muposys-0.1-alpha",3306);
$conn = new octetos\Connector();

$conn->connect($datconn);


/*
$strsql = "SELECT user,name FROM Users";
$dr = $conn->execute($strsql);

if ($dr->getNumRow() > 0) 
{
	//echo "Number : " . $dr->getNumRow() . "<br>";
	while($dr->nextRow())
	{
		echo "id: " . $dr->getString("user") . " - name: " . $dr->getString("name") . "<br>";
	}
}
else 
{
  	echo "0 results";
}
*/

//echo "Testing... 2</br>";

$users = Users::select($conn," person != 0");
$number = rand(0,10000);
$userName = "user-" . $number;
$pername = "name-" . $number;

//echo "Testing ... 3</br>";
//echo "Array : " . $users;
//echo "Size : " . sizeof($users) . "<br>";


foreach ($users as &$row) 
{
	$row->downName($conn);
	//echo "id : " .  $row->getPerson()->getEnte()->getID() . "<br>";
	echo "id : " .  $row->getPerson()->getEnte()->getID() . " - " . $row->getName() . "<br>";
	//$row->upName($conn,"root");
}

$newuser = new Users();
if($newuser->insert($conn,$userName,$pername))
{
	echo "<br>inserted $userName</br>";
}
else
{
	echo "<br>Fallo insert </br>";
}



?>
