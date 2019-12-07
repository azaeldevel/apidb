
-- -----------------------------------------------------
-- Schema sysappv2.alpha
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `sysappv2.alpha` ;
USE `sysappv2.alpha` ;

-- -----------------------------------------------------
-- Table `sysappv2.alpha`.`Persons`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `sysappv2.alpha`.`Persons` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `name1` NVARCHAR(30) NOT NULL COMMENT 'Primer nombre',
  `name2` NVARCHAR(30) NULL COMMENT 'nombres adicionales',
  `name3` NVARCHAR(30) NULL COMMENT 'Herredado del Padre',
  `name4` NVARCHAR(30) NULL COMMENT 'Heredado de la Madre',
  `country` ENUM('MEX','USA') NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `sysappv2.alpha`.`Users`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `sysappv2.alpha`.`Users` (
  `person` INT NOT NULL,
  `username` VARCHAR(16) NULL,
  `pwTx` VARCHAR(45) NULL,
  `pwMD5` VARCHAR(45) NULL,
  INDEX `fk_Users_Persons_idx` (`person` ASC),
  PRIMARY KEY (`person`),
  UNIQUE INDEX `username_UNIQUE` (`username` ASC),
  CONSTRAINT `fk_Users_Persons`
    FOREIGN KEY (`person`)
    REFERENCES `sysappv2.alpha`.`Persons` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

