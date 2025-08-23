/*
Author: Scott Field
Date 06/15/2025
Version 1.0
Purpose:
Test the DuckDB functions
*/

#include "libraries.h"
#include "stdio.h"

#define dbPath(NAME) "/home/sfcyberbox/Desktop/Software_Projects/C/DuckDB_TEST/" #NAME
#define table(NAME, COLS)      "CREATE TABLE " #NAME " (" COLS ");"
#define values(...)           __VA_ARGS__
#define insert(TABLE, VALUES) "INSERT INTO " #TABLE " VALUES " VALUES ";"
#define select(TABLE, COLS)   "SELECT " COLS " FROM " #TABLE ";"

void handleError(const char * msg, char * outError);

int main(){
  duckdb_database db;
  duckdb_connection con;
  duckdb_state state;
  duckdb_result result;
  const char * dbPath = dbPath(test.duckdb);
  char * outError = NULL;

  //Attempt to Open Database Output Any Errors
  if (duckdb_open_ext(dbPath, &db, NULL, &outError) == DuckDBError){
    //If an error occurs handle it
    handleError("Database Open Error:",outError);
    return 0;
  }else{
    puts("Database Opened Succssfully");
  }

  //Attempt to Connect to Database Output Any Errors
  if (duckdb_connect(db, &con) == DuckDBError) {
    //If an error occurs handle it
    handleError("Database Connection Error",outError);
    return 0;
  }else{
    puts("Database Connection Established Successfully");
  }

  //Attempt to Create A Table)
  state = duckdb_query(con, table(Numbers,"id INTEGER, value INTEGER"), &result);
  if (state == DuckDBError){
    handleError("Query Error", result.deprecated_error_message);
  }else{
    puts("Table Created Success");
  }

  //Attempt to Insert Data Into A Table
  state = duckdb_query(con, insert(Numbers, "(3,4), (5,6), (7, NULL)"),&result);
  if (state == DuckDBError){
    handleError("Query Error", result.deprecated_error_message);
  }else{
    puts("Insert Data Success");
  }

  //Attempt to READ Data From A TABLE
  state = duckdb_query(con, select(Numbers,"*"), &result);
  if (state == DuckDBError){
    handleError("Query Error", result.deprecated_error_message);
  }else{
    puts("Read Data Success:");
    //Output Data Using duckdb_value format
    idx_t row_count = duckdb_row_count(&result);
    idx_t column_count = duckdb_column_count(&result);
    for (idx_t row = 0; row < row_count; row++) {
        for (idx_t col = 0; col < column_count; col++) {
            if (col > 0) printf(",");
            char* str_val = duckdb_value_varchar(&result, col, row); //fixed warning
            printf("%s", str_val);
            duckdb_free(str_val);
        }
      printf("\n");
    }
  }
  //cleanup
  duckdb_disconnect(&con);
  duckdb_close(&db);
  duckdb_destroy_result(&result);
}

//Output Any Error
void handleError(const char * msg, char * outError){
  puts(msg);
  if (outError){
    puts(outError);
    duckdb_free(outError);
  }else{
    puts("No Error Information");
  }
}
