
CREATE TABLE `sysapp.alpha`.`Persons` (`id` INT NOT NULL AUTO_INCREMENT,`name1` VARCHAR(45) NOT NULL,`name2` VARCHAR(45) NULL,`name3` VARCHAR(45) NULL ,`name4` VARCHAR(45) NULL,`country` VARCHAR(45) NOT NULL, PRIMARY KEY (`id`)) ENGINE = InnoDB;

CREATE TABLE `sysapp.alpha`.`Users` (`person` INT NOT NULL PRIMARY KEY,`username` VARCHAR(16) NULL, `pwTx` VARCHAR(45) NULL, `pwMD5` VARCHAR(45) NULL,CONSTRAINT `fk_Users_Persons` FOREIGN KEY (`person`) REFERENCES `sysapp.alpha`.`Persons` (`id`)) ENGINE = InnoDB;
