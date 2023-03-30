/* Query 1
Get the supplier with the biggest amount of different provided products. Return the
supplier name and the count of different provided products.
*/


/* Query 2
Get the first five employees with the highest number of orders. Show the Employee’s
ID’s FirstName, LastName and their count of orders. Sort the result by the count of
orders descending.
*/

SELECT e.`EmployeeID`,e.`FirstName`,e.`LastName`, COUNT(distinct o.`OrderID`) as numOrd
FROM `Employees` e , `Orders` o WHERE e.`EmployeeID` = o.`EmployeeID`
GROUP BY e.`EmployeeID` ORDER BY numOrd DESC limit 5;

/* Query 3
Display all informaon stored at the database for all the products of the Supplier named 
“Grandma Kelly’s Homestead”. Use a single query (with subquery if needed).
*/



/* Query 4
Show the count of different served Products for each Order, also show the name and the 
last name of the Employee who placed the Order.
*/
-- SELECT e.`FirstName`,e.`LastName`
-- FROM `Employees` e 

SELECT * -- e.`FirstName`, e.`LastName` , COUNT(DISTINCT o.`ProductID`) as numProd
FROM `Employees` e join `Orders` o
WHERE  (e.`EmployeeID`= o.`EmployeeID`)
GROUP BY o.`OrderID`;




/* Query 5
Show the product name, quanty and average unit price for each product sold.
*/


/* Query 6
Show complete information for orders placed in 1997 and shipped in the same year by
German customers. Sort the result by customer and order date ascending.
*/

SELECT o.`OrderID`
FROM `Orders` o 
WHERE o.`OrderDate` = 1997 and o.`ShippedDate` = 1997 and o.`ShipCountry` = 'Germany'
GROUP BY o.`CustomerID` ORDER BY o.`OrderDate` DESC;


/* Query 7
Return the order idenfier and the date for all the Orders which contain Products that
belong to the “Beverages” Category. Sort the results by date descending. Do not use the
Category Idenfier, use “Beverages”.
*/


/* Query 8
Locate the Order with OrderID 10255 and calculate its total cost adding the UnitPrice of all the contained products inside it.
*/
SELECT sum(d.`ProductID` * d.`UnitPrice`) as preuTotal
FROM `OrderDetails` d
WHERE d.`OrderID`=10255;


/* Query 9
Show the complete informaon for orders containing Products from Japanese Suppliers. 
Sort the result by customer and order date ascending.
*/


/* Query 10
Show the cheapest and the most expensive product(s) (Use only a single query).
*/

SELECT MAX(d.`UnitPrice`), MIN(d.`UnitPrice`), d.`ProductID`
FROM `OrderDetails` d;



/* Query 11
Return the address, city, postal code and country of all Clients. All fields in the same
return field, that is, the previous four fields in a single column (find a funcon to help
you concatenate those values in a single field). Also show the client's idenfier and name
in two other single columns.
*/


/* Query 12
We want to know who are the employees with more processed orders than the
employee with id number 8. Show their personal information.
*/

SELECT e.`EmployeeID`
FROM `Employees` e join `Orders` o
WHERE COUNT(distinct o.`OrdersID`) > ;

/* Query 13
Show the orders with more than three different products inside.
*/


/* Query 14
Return the orders of customers located in London and for the 
suppliers in New Orleans.
*/
SELECT d.`OrderID`
FROM `OrderDetails`d ,`Suppliers` s , `Customers` c
WHERE s.`City`='New Orleans',c.`City`='London';
-- Nomes fa falta que un producte estigui fabricat a New Orleans

/* Query 5
Show the product name, quanty and average unit price for each product sold.
*/


/* Query 16
Show the complete name of the employees who processed the highest number of
orders. (Note: There could be more than one employee with the same record, so deliver
a query that accepts more than one result).
*/

SELECT e.`FirstName`,e.`LastName`
FROM `Employees` e, `Orders` o
WHERE e.`EmployeeID` = o.`EmployeeID`
GROUP BY e.`EmployeeID` ORDER BY COUNT(distinct o.`OrderID`) DESC ;


/* Query 5
Show the product name, quanty and average unit price for each product sold.
*/


/* Query 5
Show the product name, quanty and average unit price for each product sold.
*/


/* Query 5
Show the product name, quanty and average unit price for each product sold.
*/


/* Query 5
Show the product name, quanty and average unit price for each product sold.
*/


