<?php
//echo "Testing...</br>";
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);
include "octetos-db.php";
include "muposys.php";


$datconn = new octetos\Datconnect();
$datconn->create("localhost","muposys","123456","muposys-0.1-alpha",3306);
$conn = new octetos\Connector();

$conn->connect($datconn);


$number = rand(0,100000);
$userName = "user-" . $number;
$pername = "name-" . $number;
$newuser = new Users();
if($newuser->insertRaw($conn,$userName,$pername))
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

$number2 = rand(0,100000);
$userName2 = "user-" . $number2;
$pername2 = "name-" . $number2;

$newuser2 = new Users();
if($newuser2->insertRaw($conn,$userName2,$pername2))
{
	echo "inserted 2 $userName2<br>";
}
else
{
	echo "Fallo insert<br>";
}


$number3 = rand(0,100000);
$userName3 = "user-" . $number3;
$pername3 = "name-" . $number3;

$newuser3 = new Users();
if($newuser3->insertRaw($conn,$userName3,$pername3))
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
echo "newuser->getPersonValue() = " . $newuser->getPersonValue() . " <br>";

$userList = Users::select($conn," person != 0",10,"D");
foreach ($userList as &$u) 
{
	if(!$u->downName($conn))
	{
		echo "Fallo en descarga de de datos de usurio<br>";
	}
	else
	{
		//echo "descarga completada <br>";
	}
	echo "id : " .  $u->getPerson()->getEnte()->getID() . " - " . $u->getName() . "<br>";
}


$number = rand(0,100000);
$projectName = "proj-" . $number;
$proj = new SoftwareProjects();
if($proj->insertRaw($conn,$projectName))
{
	echo "inserted $projectName<br>";
}
else
{
	echo "Fallo insert $projectName <br>";
}

$where = "project = " . $proj->getEnteValue();
$rqsList = Users::select($conn,$where,1,"D");
$lastRQNumber = 1;
if(count($rqsList) > 0)
{
	$rqlast = $rqsList[0];
	if($rqlast->downNumber(conn))
	{
		$lastRQNumber = $rqlast->getNumber();
	}
	else
	{
		echo "Fallo descargade rqlast <br>";
	}
}


$number = rand(0,100000);
$rqname1 = "rq-" . $number;
$rq1 = new SoftwareRQs();
if($rq1->insertObject($conn,$proj,$lastRQNumber++,$rqname1,1))
{
	echo "inserted $rqname1<br>";
}
else
{
	echo "Fallo insert $rqname1 <br>";
}

$number = rand(0,100000);
$rqname2 = "rq-" . $number;
$rq2 = new SoftwareRQs();
if($rq2->insertObject($conn,$proj,$lastRQNumber++,$rqname2,1))
{
	echo "inserted $rqname2<br>";
}
else
{
	echo "Fallo insert $rqname2 <br>";
}

$number = rand(0,100000);
$rqname3 = "rq-" . $number;
$rq3 = new SoftwareRQs();
if($rq3->insertObject($conn,$proj,$lastRQNumber++,$rqname3,1))
{
	echo "inserted $rqname3<br>";
}
else
{
	echo "Fallo insert $rqname3 <br>";
}


?>
