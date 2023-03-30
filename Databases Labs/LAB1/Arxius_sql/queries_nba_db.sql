-- q1:
select * from Player pl, Person p where pl.Player_id = p.Id_Card and p.Nationality != 'united states';

-- q2:
select a.Arena_n, a.Name_fr, ar.City, a.Budget from Arena ar, Franchise a, Franchise b where a.Arena_n = b.Arena_n and
a.Name_fr<>b.Name_fr and a.Arena_n = ar.arena_name;

-- q3:
select P_Name, Surname, BirthDate, Victory_percent from Head_coach hd, Person p where hd.HCoach_id = p.Id_Card and hd.Victory_percent > 30
order by Victory_percent desc;

-- q4:


-- q5:
select ID_Card, P_Name, Surname, Nationality, Draft_year, Name_fr, Position from Drafted dr, Person p where dr.Draft_year = 2019;

-- q7:
select P_Name, Surname, Country from Played_Nt pnt, Person p where pnt.Player_id = p.Id_Card;

-- q8:
select * from Assistant_coach ac, Person p where ac.ACoach_id = p.Id_Card and ac.Boss_id = 0;

-- q9: 
select * from Person p1, Person p2 where p1.P_Name = p2.P_Name and p1.Surname = p2.Surname and p1.ID_Card<>p2.ID_Card;

-- q10:
select P_Name, Surname, ID_Card, N_year from Franchise f, Person p, Head_coach hc, National_team nt where f.HCoach_id = nt.HCoach_id;

-- q11:
select P_Name, Surname, Nationality, Salary, (Salary-(Salary*0.12)) as NewSalary from Person p, Player_fr pf where pf.Player_id = p.ID_Card;

-- q12: For all rookie players (those who turned PRO in the last 2 years or less) that we do not have their University of Origin or they never played for their national team, return their personal data.
select*from Person