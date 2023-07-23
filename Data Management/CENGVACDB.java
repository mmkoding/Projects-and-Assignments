package ceng.ceng351.cengvacdb;

import java.sql.*;
import java.util.ArrayList;

public class CENGVACDB implements ICENGVACDB{
    private static final String user = "e2381705";
    private static final String password = "JIhwjrdO5vQ!";
    private static final String host = "144.122.71.121";
    private static final String database = "db2381705";
    private static final int port = 8080;


	private static final String CREATE_TABLE_USER_SQL =
			"CREATE TABLE User (" +
					"userID INT," +
					"userName VARCHAR(30)," +
					"age INT," +
					"address VARCHAR(150)," +
					"password VARCHAR(30)," +
					"status VARCHAR(15)," +
					"PRIMARY KEY (userID)" +

			");";
	private static final String CREATE_TABLE_VACCINE_SQL =
			"CREATE TABLE Vaccine (" +
					"code INT," +
					"vaccinename VARCHAR(30)," +
					"type VARCHAR(30)," +
					"PRIMARY KEY (code)" +
			");";

	private static final String CREATE_TABLE_VACCINATION_SQL =
			"CREATE TABLE Vaccination (" +
					"code INT," +
					"userID INT," +
					"dose INT," +
					"vacdate DATE," +
					"PRIMARY KEY (userID, code, dose)," +
					"FOREIGN KEY (userID) REFERENCES User(userID)," +
					"FOREIGN KEY (code) REFERENCES Vaccine(code)" +
			");";

	private static final String CREATE_TABLE_ALLERGICSIDEEFFECT_SQL =
			"CREATE TABLE AllergicSideEffect (" +
					"effectcode INT," +
					"effectname VARCHAR(50)," +
					"PRIMARY KEY (effectcode)" +
			");";

	private static final String CREATE_TABLE_SEEN_SQL =
			"CREATE TABLE Seen (" +
					"effectcode INT," +
					"code INT," +
					"userID INT," +
					"date DATE," +
					"degree VARCHAR(30)," +
					"PRIMARY KEY (effectcode, code, userID)," +
					"FOREIGN KEY (effectcode) REFERENCES AllergicSideEffect(effectcode)," +
					"FOREIGN KEY (userID) REFERENCES User(userID)," +
					"FOREIGN KEY (code) REFERENCES Vaccine(code) ON DELETE CASCADE" +


			");";

	private static final String[] CREATE_TABLE_SQL_LIST = new String[]{
            CREATE_TABLE_USER_SQL,
            CREATE_TABLE_VACCINE_SQL,
            CREATE_TABLE_VACCINATION_SQL,
            CREATE_TABLE_ALLERGICSIDEEFFECT_SQL,
            CREATE_TABLE_SEEN_SQL,
    };

    private static final String[] DROP_TABLE_LIST = new String[]{
            "Seen",
            "AllergicSideEffect",
            "Vaccination",
            "Vaccine",
            "User"
    };

    private Connection conn = null;

    @Override
    public void initialize() {
        String url = "jdbc:mysql://" + host + ":" + port + "/" + database + "?autoReconnect=true&useSSL=false";

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            conn = DriverManager.getConnection(url, user, password);
        } catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    @Override
    public int createTables() {
        int createdTables = 0;

        for (String sql : CREATE_TABLE_SQL_LIST) {
            try {
                conn.prepareStatement(sql).execute();

                createdTables += 1;
            }   catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return createdTables;
    }


     @Override
    public int dropTables() {
        int droppedTables = 0;

        for (String tableName : DROP_TABLE_LIST) {
            try {
                String sql = "DROP TABLE IF EXISTS " + tableName + ";";

                conn.prepareStatement(sql).execute();

                droppedTables += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return droppedTables;
    }


    @Override
    public int insertUser(User[] users) {
        int insertedRows = 0;

        for (User user: users) {
            try {
                String sql = "INSERT INTO User (userID, userName, age, address, password, status) VALUES (?, ?, ?, ?, ?, ?);";
                PreparedStatement statement = this.conn.prepareStatement(sql);

                statement.setInt(1, user.getUserID());
                statement.setString(2, user.getUserName());
                statement.setInt(3, user.getAge());
                statement.setString(4, user.getAddress());
                statement.setString(5, user.getPassword());
                statement.setString(6, user.getStatus());

                statement.execute();

                insertedRows += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return insertedRows;
    }

    @Override
    public int insertVaccine(Vaccine[] vaccines) {
        int insertedRows = 0;

        for (Vaccine vaccine: vaccines) {
            try {
                String sql = "INSERT INTO Vaccine (code, vaccinename, type) VALUES (?, ?, ?);";
                PreparedStatement statement = this.conn.prepareStatement(sql);

                statement.setInt(1, vaccine.getCode());
                statement.setString(2, vaccine.getVaccineName());
                statement.setString(3, vaccine.getType());

                statement.execute();

                insertedRows += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return insertedRows;
    }

    @Override
    public int insertVaccination(Vaccination[] vaccinations) {
        int insertedRows = 0;

        for (Vaccination vaccination: vaccinations) {
            try {
                //String sql = "INSERT INTO Vaccination (code, userID, dose, vacdate) VALUES (?, ?, ?, ?);";
                PreparedStatement statement = this.conn.prepareStatement("INSERT INTO Vaccination (code, userID, dose, vacdate) VALUES (?, ?, ?, ?);");

                statement.setInt(1, vaccination.getCode());
                statement.setInt(2, vaccination.getUserID());
                statement.setInt(3, vaccination.getDose());
                statement.setString(4, vaccination.getVacdate());

                statement.execute();

                insertedRows += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return insertedRows;
    }

    @Override
    public int insertAllergicSideEffect(AllergicSideEffect[] allergicsideeffects) {
        int insertedRows = 0;

        for (AllergicSideEffect allergicsideeffect: allergicsideeffects) {
            try {
                String sql = "INSERT INTO AllergicSideEffect (effectcode, effectname) VALUES (?, ?);";
                PreparedStatement statement = this.conn.prepareStatement(sql);

                statement.setInt(1, allergicsideeffect.getEffectCode());
                statement.setString(2, allergicsideeffect.getEffectName());

                statement.execute();

                insertedRows += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return insertedRows;
    }

    @Override
    public int insertSeen(Seen[] seen_p) {
        int insertedRows = 0;

        for (Seen seen: seen_p) {
            try {
                String sql = "INSERT INTO Seen (effectcode, code, userID, date, degree) VALUES (?, ?, ?, ?, ?);";
                PreparedStatement statement = this.conn.prepareStatement(sql);

                statement.setInt(1, seen.getEffectcode());
                statement.setInt(2, seen.getCode());
                statement.setString(3, seen.getUserID());
                statement.setString(4, seen.getDate());
                statement.setString(5, seen.getDegree());

                statement.execute();

                insertedRows += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return insertedRows;
    }

/////////////////////////////////QUESTION-3/////////////////////////////////

    @Override
    public Vaccine[] getVaccinesNotAppliedAnyUser(){
        String sql = "SELECT DISTINCT V.code, V.vaccinename, V.type " +
        			 "FROM Vaccine V " +
        			 "WHERE V.code NOT IN ( " +
        			 "SELECT V2.code " +
        			 "FROM Vaccine V2, Vaccination O2 " +
                	 "WHERE V2.code = O2.code) " +
                	 "ORDER BY V.code";
        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<Vaccine> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new Vaccine(
                        resultSet.getInt("code"),
                        resultSet.getString("vaccinename"),
                        resultSet.getString("type")
                ));
            }

            return results.toArray(new Vaccine[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new Vaccine[0];
    }




	/////////////////////////////////QUESTION-4//////////////////////////////////
	@Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersforTwoDosesByDate(String vacdate){
        String sql = "SELECT DISTINCT U.userID, U.userName, U.address " + 
        			 "FROM User U " +
                	 "WHERE  2 <= (  SELECT COUNT(*)   "             +
                                    "FROM Vaccination O " +
                                    "WHERE U.userID = O.userID AND O.vacdate > ? " +
                                    "GROUP BY U.UserID) " +
                	 "ORDER BY U.userID ";
        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setString(1, vacdate);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<QueryResult.UserIDuserNameAddressResult> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new QueryResult.UserIDuserNameAddressResult(
                        resultSet.getString("userID"),
                        resultSet.getString("userName"),
                        resultSet.getString("address")
                ));
            }

            return results.toArray(new QueryResult.UserIDuserNameAddressResult[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new QueryResult.UserIDuserNameAddressResult[0];
    }

	/////////////////////////////////QUESTION-5//////////////////////////////////

	@Override
    public Vaccine[] getTwoRecentVaccinesDoNotContainVac(){
        String sql =
                                "   SELECT DISTINCT vacList.code, vacList.vaccinename, vacList.type " +
                                "   FROM (SELECT V.code, V.vaccinename, V.type, MAX(O.vacdate) AS maxdate " +
                                "         FROM Vaccination O, Vaccine V " +
                                "         WHERE O.code = V.code AND V.vaccinename NOT LIKE '%vac%'  " +
                                "         GROUP BY O.code " +
                                "         ORDER BY maxdate DESC " +
                                "         LIMIT 2 ) as vacList " +
                                "    ORDER BY vacList.code ";
        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<Vaccine> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new Vaccine(
                        resultSet.getInt("code"),
                        resultSet.getString("vaccinename"),
                        resultSet.getString("type")
                ));
            }

            return results.toArray(new Vaccine[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new Vaccine[0];
    }
	/////////////////////////////////QUESTION-6//////////////////////////////////
    @Override
    public QueryResult.UserIDuserNameAddressResult[] getUsersAtHasLeastTwoDoseAtMostOneSideEffect() {
        String sql = "SELECT DISTINCT U.userID, U.userName, U.address " + 
        			 "FROM User U, Vaccination O " +
                	 "WHERE U.UserID = O.userID AND O.dose >=2 AND O.userID NOT IN " +
                	 		"(SELECT S.userID FROM Seen S GROUP BY S.UserID HAVING COUNT(*)>1) " +
                	 "ORDER BY U.userID ";
        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<QueryResult.UserIDuserNameAddressResult> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new QueryResult.UserIDuserNameAddressResult(
                        resultSet.getString("userID"),
                        resultSet.getString("userName"),
                        resultSet.getString("address")
                ));
            }

            return results.toArray(new QueryResult.UserIDuserNameAddressResult[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new QueryResult.UserIDuserNameAddressResult[0];
    }

	/////////////////////////////////QUESTION-7//////////////////////////////////

	@Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersWithAllVaccinesCanCauseGivenSideEffect(String effectname) {
        String sql = "SELECT DISTINCT U.userID, U.userName, U.address " + 
        			 "FROM User U, Vaccination O " +
                	 "WHERE  U.userID = O.userID AND O.code IN ( SELECT S.code " +
                	 					"FROM Seen S, AllergicSideEffect A " +
                	 					"WHERE S.effectcode = A.effectcode AND A.effectname = ?) " +
                	 "ORDER BY U.userID ";
        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setString(1, effectname);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<QueryResult.UserIDuserNameAddressResult> results = new ArrayList<>();


            while (resultSet.next()) {
                results.add(new QueryResult.UserIDuserNameAddressResult(
                        resultSet.getString("userID"),
                        resultSet.getString("userName"),
                        resultSet.getString("address")
                ));
            }

            return results.toArray(new QueryResult.UserIDuserNameAddressResult[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new QueryResult.UserIDuserNameAddressResult[0];
    }

	/////////////////////////////////QUESTION-8//////////////////////////////////

    @Override
    public QueryResult.UserIDuserNameAddressResult[]  getUsersWithAtLeastTwoDifferentVaccineTypeByGivenInterval(String startdate, String enddate){
        String sql = "SELECT DISTINCT U.userID, U.userName, U.address " +
                "FROM User U, Vaccination O " +
                "WHERE  U.userID = O.userID AND O.vacdate >= ? AND O.vacdate <= ?  " +
                "       AND O.userID IN (SELECT newtable.userID " +
                "                        FROM (SELECT O2.userID, O2.code FROM Vaccination O2 GROUP BY O2.userID, O2.code) as newtable " +
                "                        GROUP BY newtable.userID " +
                "                        HAVING COUNT(*) > 1) " +
                "ORDER BY U.userID";
        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setString(1, startdate);
            statement.setString(2, enddate);

            ResultSet resultSet = statement.executeQuery();
            ArrayList<QueryResult.UserIDuserNameAddressResult> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new QueryResult.UserIDuserNameAddressResult(
                        resultSet.getString("userID"),
                        resultSet.getString("userName"),
                        resultSet.getString("address")
                ));
            }

            return results.toArray(new QueryResult.UserIDuserNameAddressResult[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new QueryResult.UserIDuserNameAddressResult[0];
    }
	/////////////////////////////////QUESTION-9//////////////////////////////////

	@Override
    public AllergicSideEffect[] getSideEffectsOfUserWhoHaveTwoDosesInLessThanTwentyDays() {
            String sql = "SELECT DISTINCT A.effectcode, A.effectname " +
                    "FROM Vaccination O, Vaccination O2, Seen S, AllergicSideEffect A " +
                    "WHERE S.userID = O.userID AND S.userID = O2.userID AND O.userID = O2.userID " +
                    "AND O2.dose - O.dose = 1 AND DATEDIFF(O2.vacdate, O.vacdate) < 20 AND S.effectcode = A.effectcode " +
                    "ORDER BY A.effectcode ";
            try {
                PreparedStatement statement = this.conn.prepareStatement(sql);

                ResultSet resultSet = statement.executeQuery();
                ArrayList<AllergicSideEffect> results = new ArrayList<>();

                while (resultSet.next()) {
                    results.add(new AllergicSideEffect(
                            resultSet.getInt("effectcode"),
                            resultSet.getString("effectname")
                    ));
                }

                return results.toArray(new AllergicSideEffect[0]);
            } catch (SQLException e) {
                e.printStackTrace();
            }

            return new AllergicSideEffect[0];
    }
            /////////////////////////////////QUESTION-10/////////////////////////////////

            @Override
            public double averageNumberofDosesofVaccinatedUserOverSixtyFiveYearsOld(){
                String sql = "SELECT AVG(vaclist.doseamount) as Average " +
                        "     FROM (SELECT DISTINCT ageList.userName, ageList.age, (COUNT(*)) as doseamount " +
                        "           FROM(SELECT DISTINCT * FROM User WHERE age > 65) as ageList, Vaccination O " +
                        "           WHERE O.userID = ageList.userID GROUP BY ageList.userID) AS vaclist ";
                double average = 0;

                try {
                    Statement statement = this.conn.createStatement();

                    ResultSet resultSet = statement.executeQuery(sql);
                    resultSet.next();

                    average = resultSet.getDouble("Average");


                } catch (SQLException e) {
                    e.printStackTrace();
                }
        
                return average;
            }

	/////////////////////////////////QUESTION-11/////////////////////////////////
        @Override
    public int updateStatusToEligible(String givendate){
        String sql = "UPDATE User U " +
        			 "SET U.status = 'Eligible' " +
                	 "WHERE U.userID IN " +
                			"(SELECT U.userID "  +
                			"FROM Vaccination O, (SELECT O2.userID, MAX(O2.dose) as ma " +
                            "                             FROM Vaccination O2 " +
                            "                             GROUP BY O2.userID) as maxdose " +
                			"WHERE U.userID = O.userID AND U.status <> 'Eligible' " +
                            "AND O.userID = maxdose.userID AND O.dose = maxdose.ma " +
                            "AND DATEDIFF(?, O.vacdate) > 120)";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setString(1, givendate);

            return statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return 0;
    }

	/////////////////////////////////QUESTION-12/////////////////////////////////

        @Override
    public Vaccine deleteVaccine(String vaccineName){
        String sql = "SELECT DISTINCT* FROM Vaccine WHERE vaccinename = ?";
        String qry = "SELECT* FROM Vaccine WHERE vaccinename = ?" ;

        ArrayList<Vaccine> resVaccine = new ArrayList<>();


        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setString(1,vaccineName);
            ResultSet resultSet = statement.executeQuery();

            resultSet.next();

            Integer code = resultSet.getInt("code");
            String vaccinename = resultSet.getString("vaccineName");
            String type = resultSet.getString("type");

            Vaccine object = new Vaccine(code, vaccinename, type);

            try{
                PreparedStatement statement2=this.conn.prepareStatement("DELETE FROM Vaccine " +
                                                                         "WHERE vaccineName=?");
                statement2.setString(1,vaccineName);

                statement2.executeUpdate();


            }catch (SQLException e) {
                e.printStackTrace();
            }

            resVaccine.add(object);

        } catch (SQLException e) {
            e.printStackTrace();
        }

        return resVaccine.get(0);
    }

}