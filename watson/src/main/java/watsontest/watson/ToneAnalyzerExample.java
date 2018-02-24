/**
 * Copyright 2017 IBM Corp. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 */
package watsontest.watson;

import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.SwingUtilities;
import javax.swing.plaf.synth.SynthSeparatorUI;
import javax.swing.text.html.HTMLDocument.Iterator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.gson.stream.JsonReader;
import com.ibm.watson.developer_cloud.tone_analyzer.v3.ToneAnalyzer;
import com.ibm.watson.developer_cloud.tone_analyzer.v3.model.ToneAnalysis;
import com.ibm.watson.developer_cloud.tone_analyzer.v3.model.ToneChatOptions;
import com.ibm.watson.developer_cloud.tone_analyzer.v3.model.ToneInput;
import com.ibm.watson.developer_cloud.tone_analyzer.v3.model.ToneOptions;
import com.ibm.watson.developer_cloud.tone_analyzer.v3.model.Utterance;
import com.ibm.watson.developer_cloud.tone_analyzer.v3.model.UtteranceAnalyses;

public class ToneAnalyzerExample {

	public static void main(String[] args) throws IOException, InterruptedException {

		new GUI();

		final String VERSION_DATE = "2016-05-19";
		ToneAnalyzer service = new ToneAnalyzer(VERSION_DATE);
		service.setUsernameAndPassword("8057aaf4-c5b1-44ca-8eb9-9aad96d7a3bb", "JFVWSewGJbG4");

		StringBuilder sb = new StringBuilder();

		Runtime r = Runtime.getRuntime();
		Process p = null;
		String s = "C:\\Users\\Adam\\Desktop\\testBitch\\tutorial.exe";
		// String s="C:\\Users\\Adam\\Desktop\\testBitch\\ApolloTradingBot.exe";
		p = r.exec(s);

		Thread.sleep(1000);

		File file = new File("twitter.txt");
		// File file = new File("fourchanbot_test.txt");
		BufferedReader reader = null;
		reader = new BufferedReader(new FileReader(file));
		String text = null;

		while ((text = reader.readLine()) != null)
			sb.append(text);

		text = sb.toString();

		//
		// String text = "I know the times are difficult! Our sales have been "
		// + "disappointing for the past three quarters for our data analytics "
		// + "product suite. We have a competitive data analytics product "
		// + "suite in the industry. But we need to do our job selling it! "
		// + "We need to acknowledge and fix our sales challenges. "
		// + "We can’t blame the economy for our lack of execution! " + "We are
		// missing critical sales opportunities. "
		// + "Our product is in no way inferior to the competitor products. "
		// + "Our clients are hungry for analytical tools to improve their "
		// + "business outcomes. Economy has nothing to do with it.";

		// Call the service and get the tone
		ToneOptions tonOptions = new ToneOptions.Builder().text(text).build();
		ToneAnalysis tone = service.tone(tonOptions).execute();
		System.out.println(tone);

//		JsonParser jsonParser = new JsonParser();
//		JsonObject jsonObject = (JsonObject) jsonParser.parse(tone.toString());
//		// get a String from the JSON object
//
//		String firstName = jsonObject.get("Anger").toString();
//
//		System.out.println("The first name is: " + firstName);

		
//		Object obj = new JsonParser().parse(tone.toString());
//		//JsonObject obj = new JsonObject();
//		JsonObject jo = (JsonObject) obj;
//		
//		String pageName = obj.getAsJsonObject("document_tone").getAsString().toString();
//		
//		System.out.println("test");
//		System.out.println(pageName);
//		System.out.println("test");
	}

}
