DROP SCHEMA IF EXISTS northwind;
DROP SCHEMA IF EXISTS northwind;
CREATE DATABASE IF NOT EXISTS northwind
DEFAULT CHARACTER SET 'utf8mb4'
DEFAULT COLLATE 'utf8mb4_general_ci';
USE northwind;

-- Shippers

DROP TABLE IF EXISTS `Shippers`;
CREATE TABLE `Shippers` (
  `ShipperID` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `CompanyName` varchar(40) NOT NULL,
  `Phone` varchar(24) DEFAULT NULL
);

-- Categories

DROP TABLE IF EXISTS `Categories`;
CREATE TABLE `Categories` (
  `CategoryID` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `CategoryName` varchar(15) NOT NULL,
  `Description` longtext,
  `Picture` varchar(100) DEFAULT NULL UNIQUE
);

-- Customers 

DROP TABLE IF EXISTS `Customers`;
CREATE TABLE `Customers` (
  `CustomerID` char(5) NOT NULL PRIMARY KEY,
  `CompanyName` varchar(40) NOT NULL,
  `ContactName` varchar(30) DEFAULT NULL,
  `ContactTitle` varchar(30) DEFAULT NULL,
  `Address` varchar(60) DEFAULT NULL,
  `City` varchar(15) DEFAULT NULL,
  `Region` varchar(15) DEFAULT NULL,
  `PostalCode` varchar(10) DEFAULT NULL,
  `Country` varchar(15) DEFAULT NULL,
  `Phone` varchar(24) DEFAULT NULL,
  `Fax` varchar(24) DEFAULT NULL
);

-- Employees

DROP TABLE IF EXISTS `Employees`;
CREATE TABLE `Employees` (
  `EmployeeID` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `LastName` varchar(20) NOT NULL,
  `FirstName` varchar(10) NOT NULL,
  `Title` varchar(30) DEFAULT NULL,
  `TitleOfCourtesy` varchar(25) DEFAULT NULL,
  `BirthDate` datetime DEFAULT NULL,
  `HireDate` datetime DEFAULT NULL,
  `Address` varchar(60) DEFAULT NULL,
  `City` varchar(15) DEFAULT NULL,
  `Region` varchar(15) DEFAULT NULL,
  `PostalCode` varchar(10) DEFAULT NULL,
  `Country` varchar(15) DEFAULT NULL,
  `HomePhone` varchar(24) DEFAULT NULL,
  `Extension` varchar(4) DEFAULT NULL,
  `Photo` varchar(100) DEFAULT NULL UNIQUE,
  `Notes` longtext,
  `ReportsTo` int(11) DEFAULT NULL,
  CONSTRAINT `FK_Employees_Employees` FOREIGN KEY (`ReportsTo`) REFERENCES `Employees` (`EmployeeID`)
);

-- Suppliers

DROP TABLE IF EXISTS `Suppliers`;
CREATE TABLE `Suppliers` (
  `SupplierID` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `CompanyName` varchar(40) NOT NULL,
  `ContactName` varchar(30) DEFAULT NULL,
  `ContactTitle` varchar(30) DEFAULT NULL,
  `Address` varchar(60) DEFAULT NULL,
  `City` varchar(15) DEFAULT NULL,
  `Region` varchar(15) DEFAULT NULL,
  `PostalCode` varchar(10) DEFAULT NULL,
  `Country` varchar(15) DEFAULT NULL,
  `Phone` varchar(24) DEFAULT NULL,
  `Fax` varchar(24) DEFAULT NULL,
  `HomePage` longtext
);

-- Products

DROP TABLE IF EXISTS `Products`;
CREATE TABLE `Products` (
  `ProductID` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `ProductName` varchar(40) NOT NULL,
  `SupplierID` int(11) DEFAULT NULL,
  `CategoryID` int(11) DEFAULT NULL,
  `QuantityPerUnit` varchar(20) DEFAULT NULL,
  `UnitPrice` double DEFAULT '0',
  `UnitsInStock` smallint(6) DEFAULT '0',
  `UnitsOnOrder` smallint(6) DEFAULT '0',
  `ReorderLevel` smallint(6) DEFAULT '0',
  `Discontinued` tinyint(1) NOT NULL DEFAULT '0',
  CONSTRAINT `FK_Products_Categories` FOREIGN KEY (`CategoryID`) REFERENCES `Categories` (`CategoryID`),
  CONSTRAINT `FK_Products_Suppliers` FOREIGN KEY (`SupplierID`) REFERENCES `Suppliers` (`SupplierID`)
);

-- Orders

DROP TABLE IF EXISTS `Orders`;
CREATE TABLE `Orders` (
  `OrderID` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `CustomerID` char(5) DEFAULT NULL,
  `EmployeeID` int(11) DEFAULT NULL,
  `OrderDate` datetime DEFAULT NULL,
  `RequiredDate` datetime DEFAULT NULL,
  `ShippedDate` datetime DEFAULT NULL,
  `ShipperID` int(11) DEFAULT NULL,
  `Freight` double DEFAULT '0',
  `ShipName` varchar(40) DEFAULT NULL,
  `ShipAddress` varchar(60) DEFAULT NULL,
  `ShipCity` varchar(15) DEFAULT NULL,
  `ShipRegion` varchar(15) DEFAULT NULL,
  `ShipPostalCode` varchar(10) DEFAULT NULL,
  `ShipCountry` varchar(15) DEFAULT NULL,
  CONSTRAINT `FK_Orders_Customers` FOREIGN KEY (`CustomerID`) REFERENCES `Customers` (`CustomerID`),
  CONSTRAINT `FK_Orders_Employees` FOREIGN KEY (`EmployeeID`) REFERENCES `Employees` (`EmployeeID`),
  CONSTRAINT `FK_Orders_Shippers` FOREIGN KEY (`ShipperID`) REFERENCES `Shippers` (`ShipperID`)
);

-- OrderDetails

DROP TABLE IF EXISTS `OrderDetails`;
CREATE TABLE `OrderDetails` (
  `OrderID` int(11) NOT NULL,
  `ProductID` int(11) NOT NULL,
  `UnitPrice` double NOT NULL DEFAULT '0',
  `Quantity` smallint(6) NOT NULL DEFAULT '0',
  `Discount` double NOT NULL DEFAULT '0',
  PRIMARY KEY (`OrderID`,`ProductID`),
  CONSTRAINT `FK_Order_Details_Orders` FOREIGN KEY (`OrderID`) REFERENCES `Orders` (`OrderID`),
  CONSTRAINT `FK_Order_Details_Products` FOREIGN KEY (`ProductID`) REFERENCES `Products` (`ProductID`)
);