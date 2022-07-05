-- MySQL dump 10.13  Distrib 8.0.28, for Linux (x86_64)
--
-- Host: localhost    Database: test
-- ------------------------------------------------------
-- Server version	8.0.28

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Person`
--

DROP TABLE IF EXISTS `Person`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Person` (
  `personID` int NOT NULL AUTO_INCREMENT,
  `FirstName` varchar(15) DEFAULT NULL,
  `LastName` varchar(15) DEFAULT NULL,
  `Age` int DEFAULT NULL,
  `birthday` date DEFAULT NULL,
  `score` float(5,2) NOT NULL,
  `InsertTime` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`personID`)
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Person`
--

LOCK TABLES `Person` WRITE;
/*!40000 ALTER TABLE `Person` DISABLE KEYS */;
INSERT INTO `Person` VALUES (1,'liu',NULL,22,'2022-02-08',99.90,'2022-02-08 12:32:42'),(5,'xiong','san',8,'2018-01-21',34.00,'2000-11-10 16:00:00'),(8,'xiu','a',8,'2015-11-11',66.00,'1995-12-31 16:00:00'),(9,'xiong','da',8,'2021-08-19',58.66,'2000-11-10 16:00:00'),(10,'wang','wu',14,'2017-12-01',78.55,'2006-07-10 20:00:00'),(14,'hua','teng',36,'2015-06-11',86.00,'2000-11-10 16:00:00'),(22,'ma','yun',36,'2017-09-01',36.10,'2000-11-10 16:00:00');
/*!40000 ALTER TABLE `Person` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `STUDENT`
--

DROP TABLE IF EXISTS `STUDENT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `STUDENT` (
  `sID` int NOT NULL,
  `NAME` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`sID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `STUDENT`
--

LOCK TABLES `STUDENT` WRITE;
/*!40000 ALTER TABLE `STUDENT` DISABLE KEYS */;
INSERT INTO `STUDENT` VALUES (1,'lily'),(2,'lucy'),(3,'lilei'),(4,'lilyzhou');
/*!40000 ALTER TABLE `STUDENT` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TEACHER`
--

DROP TABLE IF EXISTS `TEACHER`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `TEACHER` (
  `tID` int NOT NULL,
  `NAME` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`tID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TEACHER`
--

LOCK TABLES `TEACHER` WRITE;
/*!40000 ALTER TABLE `TEACHER` DISABLE KEYS */;
INSERT INTO `TEACHER` VALUES (1,'Math Teacher'),(2,'Chinese Teacher'),(3,'English Teacher');
/*!40000 ALTER TABLE `TEACHER` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TEACHER_STUDENT`
--

DROP TABLE IF EXISTS `TEACHER_STUDENT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `TEACHER_STUDENT` (
  `T_ID` int NOT NULL,
  `S_ID` int NOT NULL,
  PRIMARY KEY (`T_ID`,`S_ID`),
  KEY `S_ID_FK` (`S_ID`),
  CONSTRAINT `S_ID_FK` FOREIGN KEY (`S_ID`) REFERENCES `STUDENT` (`sID`),
  CONSTRAINT `T_ID_FK` FOREIGN KEY (`T_ID`) REFERENCES `TEACHER` (`tID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TEACHER_STUDENT`
--

LOCK TABLES `TEACHER_STUDENT` WRITE;
/*!40000 ALTER TABLE `TEACHER_STUDENT` DISABLE KEYS */;
INSERT INTO `TEACHER_STUDENT` VALUES (1,1);
/*!40000 ALTER TABLE `TEACHER_STUDENT` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pets`
--

DROP TABLE IF EXISTS `pets`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `pets` (
  `petID` int NOT NULL AUTO_INCREMENT,
  `name` varchar(10) DEFAULT NULL,
  `personID` int NOT NULL,
  PRIMARY KEY (`petID`),
  KEY `pets_ibfk_2` (`personID`),
  CONSTRAINT `pets_ibfk_2` FOREIGN KEY (`personID`) REFERENCES `Person` (`personID`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pets`
--

LOCK TABLES `pets` WRITE;
/*!40000 ALTER TABLE `pets` DISABLE KEYS */;
INSERT INTO `pets` VALUES (1,'dog',8),(2,'cat',10),(4,'rabbit',8),(7,'fish',14);
/*!40000 ALTER TABLE `pets` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-02-09  0:47:45
