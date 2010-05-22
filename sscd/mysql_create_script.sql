SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `ssc` DEFAULT CHARACTER SET utf8 ;
USE `ssc`;

-- -----------------------------------------------------
-- Table `ssc`.`Language`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `ssc`.`Language` (
  `LanguageId` VARCHAR(3) NOT NULL ,
  `Name` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`LanguageId`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ssc`.`User`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `ssc`.`User` (
  `UserId` INT NOT NULL AUTO_INCREMENT ,
  `Surname` VARCHAR(80) NOT NULL ,
  `Forename` VARCHAR(80) NOT NULL ,
  `Sex` CHAR(1) NOT NULL ,
  `BirthYear` INT NOT NULL ,
  `ZIPCode` VARCHAR(30) NOT NULL ,
  `Education` TEXT NOT NULL ,
  `Occupation` VARCHAR(128) NOT NULL COMMENT '	' ,
  `Diagnosis` VARCHAR(128) NOT NULL ,
  `Orientation` INT NOT NULL ,
  `Communication` INT NOT NULL ,
  `MotorFunction` INT NOT NULL ,
  `MouthMotoric` TEXT NOT NULL ,
  `InterviewPossible` TINYINT(1) NOT NULL ,
  `RepeatPossible` TINYINT(1) NOT NULL ,
  `MotherTongue` VARCHAR(3) NOT NULL ,
  PRIMARY KEY (`UserId`) ,
  INDEX `fk_MotherTongue` (`MotherTongue` ASC) ,
  CONSTRAINT `fk_MotherTongue`
    FOREIGN KEY (`MotherTongue` )
    REFERENCES `ssc`.`Language` (`LanguageId` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ssc`.`Institution`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `ssc`.`Institution` (
  `InstitutionId` INT NOT NULL AUTO_INCREMENT ,
  `Name` VARCHAR(128) NOT NULL ,
  PRIMARY KEY (`InstitutionId`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ssc`.`UserInInstitution`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `ssc`.`UserInInstitution` (
  `UserId` INT NOT NULL ,
  `InstitutionId` INT NOT NULL ,
  `InstitutionReferenceId` VARCHAR(64) NULL ,
  PRIMARY KEY (`UserId`, `InstitutionId`) ,
  INDEX `fk_UserId` (`UserId` ASC) ,
  INDEX `fk_InstitutionId` (`InstitutionId` ASC) ,
  CONSTRAINT `fk_UserId`
    FOREIGN KEY (`UserId` )
    REFERENCES `ssc`.`User` (`UserId` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_InstitutionId`
    FOREIGN KEY (`InstitutionId` )
    REFERENCES `ssc`.`Institution` (`InstitutionId` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ssc`.`SampleType`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `ssc`.`SampleType` (
  `SampleTypeId` INT NOT NULL AUTO_INCREMENT ,
  `Name` VARCHAR(45) NOT NULL ,
  `ExactlyRepeated` TINYINT(1) NOT NULL ,
  PRIMARY KEY (`SampleTypeId`) )
ENGINE = InnoDB;

INSERT INTO `ssc`.`SampleType` (
  `Name`, `ExactlyRepeated`) VALUES 
  ('Repeating', 1), ('Training', 1), 
  ('Interview', 0);


-- -----------------------------------------------------
-- Table `ssc`.`Microphone`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `ssc`.`Microphone` (
  `MicrophoneId` INT NOT NULL PRIMARY KEY AUTO_INCREMENT ,
  `Model` VARCHAR(100) NOT NULL ,
  `Type` VARCHAR(100) NOT NULL )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ssc`.`SoundCard`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `ssc`.`SoundCard` (
  `SoundCardId` INT NOT NULL PRIMARY KEY AUTO_INCREMENT ,
  `Model` VARCHAR(100) NOT NULL ,
  `Type` VARCHAR(100) NOT NULL )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ssc`.`Sample`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `ssc`.`Sample` (
  `SampleId` INT NOT NULL AUTO_INCREMENT ,
  `TypeId` INT NOT NULL ,
  `UserId` INT NOT NULL ,
  `MicrophoneId` INT NOT NULL ,
  `SoundCardId` INT NOT NULL ,
  `Path` VARCHAR(192) NOT NULL ,
  `Prompt` VARCHAR(256) NOT NULL ,
  PRIMARY KEY (`SampleId`) ,
  INDEX `fk_Type` (`TypeId` ASC) ,
  INDEX `fk_User` (`UserId` ASC) ,
  INDEX `fk_Microphone` (`MicrophoneId` ASC) ,
  INDEX `fk_SoundCard` (`SoundCardId` ASC) ,
  CONSTRAINT `fk_Type`
    FOREIGN KEY (`TypeId` )
    REFERENCES `ssc`.`SampleType` (`SampleTypeId` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_User`
    FOREIGN KEY (`UserId` )
    REFERENCES `ssc`.`User` (`UserId` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Microphone`
    FOREIGN KEY (`MicrophoneId` )
    REFERENCES `ssc`.`Microphone` (`MicrophoneId` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_SoundCard`
    FOREIGN KEY (`SoundCardId` )
    REFERENCES `ssc`.`SoundCard` (`SoundCardId` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


;



INSERT INTO Language (LanguageId, Name) VALUES ("de", "German"),  ("aa", "Afar"),  ("ab", "Abkhazian"),  ("af", "Afrikaans"),  ("ak", "Akan"),  ("am", "Amharic"),  ("ar", "Arabic"),  ("an", "Aragonese"),  ("as", "Assamese"),  ("av", "Avaric"),  ("ae", "Avestan"),  ("ay", "Aymara"),  ("az", "Azerbaijani"),  ("ba", "Bashkir"),  ("bm", "Bambara"),  ("be", "Belarusian"),  ("bn", "Bengali"),  ("bh", "Bihari"),  ("bi", "Bislama"),  ("bs", "Bosnian"),  ("br", "Breton"),  ("bg", "Bulgarian"),  ("ca", "Catalan"),  ("ch", "Chamorro"),  ("ce", "Chechen"),  ("cu", "Church"),  ("cv", "Chuvash"),  ("kw", "Cornish"),  ("co", "Corsican"),  ("cr", "Cree"),  ("da", "Danish"),  ("dv", "Divehi"),  ("dz", "Dzongkha"),  ("en", "English"),  ("eo", "Esperanto"),  ("et", "Estonian"),  ("ee", "Ewe"),  ("fo", "Faroese"),  ("fj", "Fijian"),  ("fi", "Finnish"),  ("fy", "Western"),  ("ff", "Fulah"),  ("gd", "Gaelic"),  ("ga", "Irish"),  ("gl", "Galician"),  ("gv", "Manx"),  ("gn", "Guarani"),  ("gu", "Gujarati"),  ("ht", "Haitian"),  ("ha", "Hausa"),  ("he", "Hebrew"),  ("hz", "Herero"),  ("hi", "Hindi"),  ("ho", "Hiri"),  ("hr", "Croatian"),  ("hu", "Hungarian"),  ("ig", "Igbo"),  ("io", "Ido"),  ("ii", "Sichuan"),  ("iu", "Inuktitut"),  ("ie", "Interlingue"),  ("ia", "Interlingua"),  ("id", "Indonesian"),  ("ik", "Inupiaq"),  ("it", "Italian"),  ("jv", "Javanese"),  ("ja", "Japanese"),  ("kl", "Kalaallisut"),  ("kn", "Kannada"),  ("ks", "Kashmiri"),  ("kr", "Kanuri"),  ("kk", "Kazakh"),  ("km", "Central"),  ("ki", "Kikuyu"),  ("rw", "Kinyarwanda"),  ("ky", "Kirghiz"),  ("kv", "Komi"),  ("kg", "Kongo"),  ("ko", "Korean"),  ("kj", "Kuanyama"),  ("ku", "Kurdish"),  ("lo", "Lao"),  ("la", "Latin"),  ("lv", "Latvian"),  ("li", "Limburgan"),  ("ln", "Lingala"),  ("lt", "Lithuanian"),  ("lb", "Luxembourgish"),  ("lu", "Luba-Katanga"),  ("lg", "Ganda"),  ("mh", "Marshallese"),  ("ml", "Malayalam"),  ("mr", "Marathi"),  ("mg", "Malagasy"),  ("mt", "Maltese"),  ("mn", "Mongolian"),  ("na", "Nauru"),  ("nv", "Navajo"),  ("nr", "Ndebele"),  ("nd", "Ndebele"),  ("ng", "Ndonga"),  ("ne", "Nepali"),  ("nn", "Norwegian"),  ("nb", "Bokmål"),  ("no", "Norwegian"),  ("ny", "Chichewa"),  ("oc", "Occitan"),  ("oj", "Ojibwa"),  ("or", "Oriya"),  ("om", "Oromo"),  ("os", "Ossetian"),  ("pa", "Panjabi"),  ("pi", "Pali"),  ("pl", "Polish"),  ("pt", "Portuguese"),  ("ps", "Pushto"),  ("qu", "Quechua"),  ("rm", "Romansh"),  ("rn", "Rundi"),  ("ru", "Russian"),  ("sg", "Sango"),  ("sa", "Sanskrit"),  ("si", "Sinhala"),  ("sl", "Slovenian"),  ("se", "Northern"),  ("sm", "Samoan"),  ("sn", "Shona"),  ("sd", "Sindhi"),  ("so", "Somali"),  ("st", "Sotho"),  ("es", "Spanish"),  ("sc", "Sardinian"),  ("sr", "Serbian"),  ("ss", "Swati"),  ("su", "Sundanese"),  ("sw", "Swahili"),  ("sv", "Swedish"),  ("ty", "Tahitian"),  ("ta", "Tamil"),  ("tt", "Tatar"),  ("te", "Telugu"),  ("tg", "Tajik"),  ("tl", "Tagalog"),  ("th", "Thai"),  ("ti", "Tigrinya"),  ("to", "Tonga"),  ("tn", "Tswana"),  ("ts", "Tsonga"),  ("tk", "Turkmen"),  ("tr", "Turkish"),  ("tw", "Twi"),  ("ug", "Uighur"),  ("uk", "Ukrainian"),  ("ur", "Urdu"),  ("uz", "Uzbek"),  ("ve", "Venda"),  ("vi", "Vietnamese"),  ("vo", "Volapük"),  ("wa", "Walloon"),  ("wo", "Wolof"),  ("xh", "Xhosa"),  ("yi", "Yiddish"),  ("yo", "Yoruba"),  ("za", "Zhuang"),  ("zu", "Zulu");









CREATE USER sscuser IDENTIFIED BY 'BHB5U3Z86T';

grant all privileges on ssc.* to sscuser identified by "BHB5U3Z86T";


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
