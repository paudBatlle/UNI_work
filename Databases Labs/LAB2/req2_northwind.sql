USE northwind;
drop procedure if exists CheckDates;
DELIMITER $$
create procedure northwind.CheckDates 
	 ( `OrderDate` date ,`RequiredDate` date, `ShippedDate` date , `OrderID` int )
   
	begin
		if ( `ShippedDate` <  o.`RequiredDate`) then
			-- select o.`OrderID` as ID;
			update `Orders`
				set o.`ShippedDate` = o.`RequiredDate`
				where o.`OrderID` = `OrderID`;
        end if;

		if o.`OrderDate` < o.`ShippedDate` then
			update `Orders`
				set o.`OrderDate` =  concat( extract( year from o.`ShippedDate`) ,'-', extract(month from o.`ShippedDate`),'-' ,(extract(day from o.`ShippedDate`)-1),' ',extract(hour from o.`ShippedDate`))
                where o.`OrderID` = `OrderID`;
		end if;
        
		insert into `Orders_Log` (`OrderID`,LogTime) values (`OrderID`, NOW());
    
	end$$
    DELIMITER $$


CALL CheckDates ();
    select * from `OrdersLog`
    
	