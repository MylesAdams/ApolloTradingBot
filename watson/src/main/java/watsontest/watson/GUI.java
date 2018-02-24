//Imports are listed in full to show what's being used
//could just import javax.swing.* and java.awt.* etc..
package watsontest.watson;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JComboBox;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JList;
import java.awt.BorderLayout;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class GUI extends JFrame{
 
 

 public GUI()
 {
	    {
	        //headers for the table
	        String[] columns = new String[] {
	            "Source", "Anger","Disgust", "Fear","Joy", "Sadness"
	        };
	         
	        //actual data for the table in a 2d array
	        Object[][] data = new Object[][] {
	            {"4Chan", 7, 40.0, 3, 100, 9},
	            {"Reddit", 7, 70.0, 3, 100, 9 },
	            {"Twitter", 7, 60.0, 3, 100, 9 },
	        };
	        //create table with data
	        JTable table = new JTable(data, columns);
	         
	        //add the table to the frame
	        this.add(new JScrollPane(table));
	         
	        this.setTitle("Table Example");
	        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);       
	        this.pack();
	        this.setVisible(true);
	    }
 }
// JFrame guiFrame = new JFrame();
// 
// //make sure the program exits when the frame closes
// guiFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
// guiFrame.setTitle("Example GUI");
// guiFrame.setSize(300,250);
// 
// //This will center the JFrame in the middle of the screen
// guiFrame.setLocationRelativeTo(null);
// 
// //Options for the JComboBox 
// String[] fruitOptions = {"Apple", "Apricot", "Banana"
// ,"Cherry", "Date", "Kiwi", "Orange", "Pear", "Strawberry"};
// 
// //Options for the JList
// String[] vegOptions = {"Asparagus", "Beans", "Broccoli", "Cabbage"
// , "Carrot", "Celery", "Cucumber", "Leek", "Mushroom"
// , "Pepper", "Radish", "Shallot", "Spinach", "Swede"
// , "Turnip"};
// 
// //The first JPanel contains a JLabel and JCombobox
// final JPanel comboPanel = new JPanel();
// JLabel comboLbl = new JLabel("Fruits:");
// JComboBox fruits = new JComboBox(fruitOptions);
// 
// comboPanel.add(comboLbl);
// comboPanel.add(fruits);
// 
// //Create the second JPanel. Add a JLabel and JList and
// //make use the JPanel is not visible.
// final JPanel listPanel = new JPanel();
// listPanel.setVisible(false);
// JLabel listLbl = new JLabel("Vegetables:");
// JList vegs = new JList(vegOptions);
// vegs.setLayoutOrientation(JList.HORIZONTAL_WRAP);
// 
// listPanel.add(listLbl);
// listPanel.add(vegs);
// 
// JButton vegFruitBut = new JButton( "Fruit or Veg");
// 
// //The ActionListener class is used to handle the
// //event that happens when the user clicks the button.
// //As there is not a lot that needs to happen we can 
// //define an anonymous inner class to make the code simpler.
// vegFruitBut.addActionListener(new ActionListener()
// {
// public void actionPerformed(ActionEvent event)
// {
// //When the fruit of veg button is pressed
// //the setVisible value of the listPanel and
// //comboPanel is switched from true to 
// //value or vice versa.
// listPanel.setVisible(!listPanel.isVisible());
// comboPanel.setVisible(!comboPanel.isVisible());
//
// }
// });
// 
// //The JFrame uses the BorderLayout layout manager.
// //Put the two JPanels and JButton in different areas.
// guiFrame.add(comboPanel, BorderLayout.NORTH);
// guiFrame.add(listPanel, BorderLayout.CENTER);
// guiFrame.add(vegFruitBut,BorderLayout.SOUTH);
// 
// //make sure the JFrame is visible
// guiFrame.setVisible(true);
// }
// 
}
