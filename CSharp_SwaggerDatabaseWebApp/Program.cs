using Microsoft.OpenApi.Models;
using PizzaStore.Models;
using Microsoft.EntityFrameworkCore;

var builder = WebApplication.CreateBuilder(args);
var connectionString = builder.Configuration.GetConnectionString("Pizzas") ?? "Data Source=Pizzas.db";

builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSqlite<PizzaDb>(connectionString);
builder.Services.AddSwaggerGen(c =>
{
     c.SwaggerDoc("v1", new OpenApiInfo {
         Title = "PizzaStore API",
         Description = "Making the Pizzas you love",
         Version = "v1" });
});

var app = builder.Build();

if (app.Environment.IsDevelopment())
{
   app.UseSwagger();
   app.UseSwaggerUI(c =>
   {
      c.SwaggerEndpoint("/swagger/v1/swagger.json", "PizzaStore API V1");
   });
}
//Get All From Database
// Test Server Syntax: Invoke-RestMethod -Method Get -Uri "http://localhost:PORT/pizzas"
app.MapGet("/pizzas", async (PizzaDb db) => await db.Pizzas.ToListAsync());

//Post To Database
// Test Server Syntax: Invoke-RestMethod -Method Post -Uri "http://localhost:PORT/pizza" -Body '{"name":"Pepperoni","description":"A classic pepperoni pizza"}' -ContentType 'application/json'
app.MapPost("/pizza", async (PizzaDb db, Pizza pizza) =>
{
    await db.Pizzas.AddAsync(pizza);
    await db.SaveChangesAsync();
    return Results.Created($"/pizza/{pizza.Id}", pizza);
});

//Get Single Record From Database
// Test Server Syntax: Invoke-RestMethod -Method Get -Uri "http://localhost:PORT/pizza/ID"
app.MapGet("/pizza/{id}", async (PizzaDb db, int id) => await db.Pizzas.FindAsync(id));

//PUT Record In Database
// Test Server Syntax
app.MapPut("/pizza/{id}", async (PizzaDb db, Pizza updatepizza, int id) =>
{
      var pizza = await db.Pizzas.FindAsync(id);
      if (pizza is null) return Results.NotFound();
      pizza.Name = updatepizza.Name;
      pizza.Description = updatepizza.Description;
      await db.SaveChangesAsync();
      return Results.NoContent();
});

//DELETE Record in Database
app.MapDelete("/pizza/{id}", async (PizzaDb db, int id) => 
{
    var pizza = await db.Pizzas.FindAsync(id);
    if (pizza is null)
    {
        return Results.NotFound();
    }
    db.Pizzas.Remove(pizza);
    await db.SaveChangesAsync();
    return Results.Ok();
});

app.Run();
