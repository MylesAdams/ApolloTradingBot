import java.util.List;

import com.sun.org.apache.xerces.internal.util.Status;

import twitter4j.TwitterException;
import twitter4j.TwitterFactory;
import twitter4j.conf.ConfigurationBuilder;

public class ApolloTwitter {

	public static void main(String[] args) throws TwitterException {
		ConfigurationBuilder cb = new ConfigurationBuilder();

		cb.setDebugEnabled(true).setOAuthConsumerKey("aCsTxs4JqEoea70c4NGY3gSwf")
				.setOAuthConsumerSecret("kWX1DunM5I9KIA6dlaGQF3GtGlFEtz06hgqOz7hr2Ev8MfDMq3")
				.setOAuthAccessToken("955967890301190144-44eow3rBOCJ27jqq6Mis8vzdOCFOH3V")
				.setOAuthAccessTokenSecret("guJGTW8JOhgkpoDx6VaHZeKwGwwJbFLXFvOFjxrot9GdQ");

		TwitterFactory tf = new TwitterFactory(cb.build());
		twitter4j.Twitter twitter = tf.getInstance();

		// get username, status

		List<twitter4j.Status> status = twitter.getHomeTimeline();
		for (twitter4j.Status st : status) {
			System.out.println(st.getCreatedAt()+"  "+ st.getUser().getName() + "----------" + st.getText());
		}
	}

}
