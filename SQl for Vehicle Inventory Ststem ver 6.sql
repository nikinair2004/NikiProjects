use inventory
-- Table structure for table `vehicledata`
CREATE TABLE `vehicledata` 
(
  `id`         int(10) NOT NULL,
  `category`   varchar(20),
   `make`      varchar(20),
   `colour`    varchar(20),
   `mileage`   int(5),
    `year`     int(4)
) ;
ALTER TABLE `vehicledata`
ADD PRIMARY KEY (`ID`);

INSERT INTO `vehicledata` (`id`, `category`, `make`,`colour`,`mileage`,`year`) VALUES
(1, 'sedan','chevrolet','yellow','30', '2019'),
(2, 'truck','ford','red', '40','2018'),
(3, 'suv','toyota','blue', '50','2019'),
(4, 'minivan','chryster','red','20', '2017')
(5, 'sedan','chevrolet','blue','30', '2016');
commit;


select * from `vehicledata`;