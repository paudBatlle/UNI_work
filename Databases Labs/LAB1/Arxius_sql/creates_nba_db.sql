DROP SCHEMA IF EXISTS nba_db;
CREATE DATABASE IF NOT EXISTS nba_db
DEFAULT CHARACTER SET 'utf8mb4'
DEFAULT COLLATE 'utf8mb4_general_ci';
USE nba_db;

CREATE TABLE Person (
	Id_Card int (9),
    P_Name varchar (20),
    Surname varchar (20),
    Nationality varchar (20),
    Gender varchar(2),
    BirthDate varchar(10),
    PRIMARY KEY (Id_Card)
);
CREATE TABLE Head_coach(
    HCoach_id int (9),
    Victory_percent float(4,2),
    Salary float (10,2),
    PRIMARY KEY (HCoach_id),
    FOREIGN KEY (HCoach_id) REFERENCES Person(Id_Card)
);

CREATE TABLE Assistant_coach (
    ACoach_id int (9),
    Especiality varchar(20),
    Boss_id int(9) DEFAULT NULL,
    PRIMARY KEY (ACoach_id),
    FOREIGN KEY (ACoach_id) REFERENCES Person(Id_Card)
);

CREATE TABLE Player (
    Player_id int(9),
    University varchar(12),
    NBA_champ int(2),
    Year_pro int(4),
    PRIMARY KEY (Player_id),
    FOREIGN KEY (Player_id) REFERENCES Person(Id_Card)
);


CREATE TABLE National_team (
	Country varchar(20),
    HCoach_id int(9),
    N_year int(4),
    PRIMARY KEY (Country),
    FOREIGN KEY (HCoach_id) REFERENCES Head_coach(HCoach_id)
);

CREATE TABLE Arena (
    Arena_name varchar(50),
    City varchar(50),
    Capacity int(8),
    PRIMARY KEY (Arena_name)
);

CREATE TABLE Franchise (
	Name_fr varchar(50),
    Geolocation varchar(4),
    Arena_n varchar(50),
    HCoach_id int(9),
    NBA_rings int(2),
    Budget int(9),
    PRIMARY KEY (Name_fr),
    FOREIGN KEY (HCoach_id) REFERENCES Head_coach(HCoach_id),
    FOREIGN KEY (Arena_n) REFERENCES Arena(Arena_name)
);

CREATE TABLE Drafted (
	Draft_year int(4),
    Player_id int(9),
    Name_fr varchar(50),
    Position int(2),
    PRIMARY KEY (Player_id),
    FOREIGN KEY (Player_id) REFERENCES Player(Player_id),
    FOREIGN KEY (Name_fr) REFERENCES Franchise(Name_fr)
);

CREATE TABLE Player_Fr (
	Player_id int(9),
    Name_fr varchar(50),
    Shirt_number int(3),
    Start_year date,
    End_year date,
    Salary float (12,2),
    PRIMARY KEY (Player_id),
    FOREIGN KEY (Player_id) REFERENCES Player(Player_id),
    FOREIGN KEY (Name_fr) REFERENCES Franchise(Name_fr)
);

CREATE TABLE Played_Nt (
    Year_nt int(4),
    Country varchar(20),
    Player_id int(9),
    Shirt_number int(3),
    PRIMARY KEY (Player_id),
    FOREIGN KEY (Player_id) REFERENCES Player(Player_id),
    FOREIGN KEY (Country) REFERENCES National_team(Country)
);
CREATE TABLE Conference(
	Name_conf varchar(50),
    PRIMARY KEY (Name_conf)
);
CREATE TABLE Plays_in(
	Year_play int(4),
    Start_year date,
    End_year date,
    Name_fr varchar(50),
    Name_conf varchar(50),
    RegularSeasonYear int(4),
    IsWinner int(1),
    PRIMARY KEY (Name_fr),
    FOREIGN KEY (Name_fr) REFERENCES Franchise(Name_fr),
    FOREIGN KEY (Name_conf) REFERENCES Conference(Name_conf)
); 


CREATE TABLE Zone (
	Zone_Code int(3),
	Arena_name varchar(50),
    is_VIP int(1),
    PRIMARY KEY (Zone_Code),
    FOREIGN KEY (Arena_name) REFERENCES Arena(Arena_name)
);

CREATE TABLE Seats(
	Arena_name varchar(50),
    Zone_Code int(3),
    Number_seats int(10),
    Color varchar(15),
    PRIMARY KEY (Number_seats),
    FOREIGN KEY (Arena_name) REFERENCES Arena(Arena_name),
    FOREIGN KEY (Zone_Code) REFERENCES Zone(Zone_Code)
);

