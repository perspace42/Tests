using C__Example_ASP_Web_App_Project.Models;
using C__Example_ASP_Web_App_Project.Services;
using Microsoft.AspNetCore.Mvc;

namespace C__Example_ASP_Web_App_Project.Controllers;

[ApiController]
[Route("[controller]")]
public class PizzaController : ControllerBase
{
    public PizzaController()
    {
    }

    // GET all action (Retrieves All From Cache)
    [HttpGet]
    public ActionResult<List<Pizza>> GetAll() =>
        PizzaService.GetAll();

    // GET by Id action (Retrieves Specific From Cache)
    [HttpGet("{id}")]
    public ActionResult<Pizza> Get(int id)
    {
        var pizza = PizzaService.Get(id);
        if(pizza == null)
            return NotFound();
        return pizza;
    }

    // POST action (Creates and Adds To Cache)
    /*
    EXAMPLE SYNTAX FOR POST:
    https://localhost:{HOST}/> post /Pizza -c "{"name":"Hawaii", "isGlutenFree":false}" 
    */
    [HttpPost]
    public IActionResult Create(Pizza pizza)
    {      
        // This code will save the pizza and return a result
        PizzaService.Add(pizza);
        return CreatedAtAction(nameof(Get), new { id = pizza.Id }, pizza);
    }

    // PUT action (Modifies Existing, and Updates Cache)
    /*
    EXAMPLE SYNTAX FOR PUT:
    https://localhost:{HOST}/> put /Pizza/3 -c "{"id": 3, "name":"Hawaiian", "isGlutenFree":false}"
    */
    [HttpPut("{id}")]
    public IActionResult Update(int id, Pizza pizza)
    {
        // This code will update the pizza and return a result
        if (id != pizza.Id)
        return BadRequest();
           
        var existingPizza = PizzaService.Get(id);
        if(existingPizza is null)
            return NotFound();
    
        PizzaService.Update(pizza);           
    
        return NoContent();
    }

    // DELETE action (Removes Existing, Clears From Cache)
    /*
    EXAMPLE SYNTAX FOR DELETE:
    https://localhost:{HOST}/> DELETE /pizza/3
    */
    [HttpDelete("{id}")]
    public IActionResult Delete(int id)
    {
        // This code will delete the pizza and return a result
        var pizza = PizzaService.Get(id);
   
        if (pizza is null)
            return NotFound();
        
        PizzaService.Delete(id);
    
        return NoContent();
    }
}