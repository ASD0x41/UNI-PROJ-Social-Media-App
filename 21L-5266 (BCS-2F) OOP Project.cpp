/////////////////////////	Muhammad Asad Tariq (21L-5266) --- BCS-2F	/////////////////////////
/////////////////////////	   Object-Oriented Programming Project		////////////////////////


#include <iostream>
#include <fstream>

using namespace std;



/////////////////////////	Constants:		/////////////////////////


const int BUFFER_SIZE = 100;
const int MAX_ARRAY_SIZE = 10;



/////////////////////////	Templates:		/////////////////////////


template <typename T>
void AddToList(T**& list, T* item)
{
	if (list == NULL)	// i.e. if first titem in list
	{
		list = new T*[MAX_ARRAY_SIZE];
		for (int i = 0; i < MAX_ARRAY_SIZE; i++)
			list[i] = NULL;
	}

	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
		if (list[i] == NULL)	// First empty slot
		{
			list[i] = item;
			return;
		}
}

template <typename T>
void Deallocate(T** list, int size)
{
	for (int i = 0; i < size; i++)
		delete list[i];
	delete[] list;
}

template <typename T>
void Deallocate(T** list)
{
	for (int i = 0; list[i] != NULL; i++)
		delete list[i];
	delete[] list;
}

void Deallocate(char*** list, int size)
{
	for (int i = 0; i < size; i++)
		if (list[i] != NULL)
		{
			for (int j = 0; list[i][j] != NULL; j++)
				delete[] list[i][j];
			delete[] list[i];
		}
	delete[] list;
}



/////////////////////////	Class Declarations:		/////////////////////////


class Date;				// Data type for use in other classes

class Facebook;			// Singleton Controller Class

class Object;			// Abstract Base Class - For applying polymorphism to User & Page classes
class User;				// Derived from Object
class Page;				// Derived from Object

class Post;
class Memory;			// Derived from Post

class Activity;
class Comment;

class Helper;			// General Functions for use throughout the program



/////////////////////////	Class Definitions:	/////////////////////////
///// (including function prototypes only - definitions below main)	/////


class Date
{
	/////// Friends of Class Date:

	friend ostream& operator<<(ostream& out, const Date& date);		// Overloaded stream extraction operator for displaying date on console using cout command


private:

	/////// Data Members:

	int day;
	int month;
	int year;


public:

	static Date currentDate;

	/////// Constructors/Destructors:

	Date(const int& _day = 1, const int& _month = 6, const int& _year = 2022);		// No need for creating special destructor because no dynamically allocated data in class

	/////// Getters/Setters:

	void SetDate(const int& _day, const int& _month, const int& _year);
	void SetDate(const Date& rhs);


	/////// Other Member Functions:

	bool CheckDateRange(const Date& rhs, const int& days);		// For checking whether a certain date is within a certain range from another date
	bool IsDayMonthSame(const Date& rhs);						// For checking whether two dates differ only in the year while the day and month are the same

	int YearDifference(const Date& rhs);						// For finding the difference between the years of two dates
};

Date Date::currentDate;		// In older versions of C++, we need a definition of static data members in addition to their declarations inside the class

class Facebook
{
private:

	/////// Data Members:

	User* currentUser;

	Page** pages;
	User** users;
	Post** posts;

	int totalPages;
	int totalUsers;
	int totalPosts;

	int totalComments;		// For generating IDs for new comments
	int latestMemoryID;		// For generating IDs for new memories


	/////// Utility Functions:

	Object* SearchObjectByID(char* ID);
	Page* SearchPageByID(char* ID);
	User* SearchUserByID(char* ID);
	Post* SearchPostByID(char* ID);


public:

	/////// Constructors/Destructors:

	Facebook();
	~Facebook();


	/////// Getters/Setters:

	void SetCurrentDate(int day, int month, int year);
	void SetCurrentUser(char* user);


	/////// Other Functions:

	void LoadData(char* pagefile, char* userfile, char* postfile, char* commentfile);

	void LoadPages(char* pagefile);
	void LoadUsers(char* userfile);
	void LoadPosts(char* postfile);
	void LoadComments(char* commentfile);

	void Run(char* user);

	void ViewFriendList();
	void ViewLikedPages();
	void ViewHomePage();
	void ViewTimeLine();
	void SeeYourMemories();

	void LikePost(char* post);
	void ShareMemory(char* post, char* text);
	void PostComment(char* post, char* comment);

	void ViewLikedList(char* post);
	void ViewPost(char* post);

	void ViewPage(char* page);
};

class Object
{
protected:

	/////// Data Members:

	char* ID;

	/////// Constructors/Destructors:

	~Object();

public:

	Object();


	/////// Other Functions:			//	pure virtual because Object is an abstract class made just for the sake of polymorphism with Page and User

	virtual char* GetName() = 0;											// To get name (first and last) of user or title of page

	virtual void ViewTimeLine() = 0;										// Display all posts of a user or page
	virtual void ViewRecentPosts() = 0;
	virtual void PrintListView() = 0;										// Display ID and name of user or page

	virtual void AddToTimeline(Post* post) = 0;								// Add a post to the timeline of a user or page
};

class User : public Object
{
private:

	char* fName;
	char* lName;

	User** friendUsers;
	Page** likedPages;

	Post** timeline;

public:

	User();

	char* GetID();
	char* GetfName();
	char* GetlName();
	char* GetName();

	void AddToTimeline(Post* post);
	void ReadUserFromFile(ifstream& fin, char**& tempFriends);
	void LikePage(Page*& page);		// To form association between users and liked pages
	void AddFriend(User*& user);	// To form association between users and friend users
	void ViewFriendsList();
	void ViewLikedPages();
	void PrintListView();
	void ViewHomePage();
	void ViewRecentPosts();
	void ViewTimeLine();
	void DisplayMemories();

	~User();
};

class Page : public Object
{
private:

	char* title;
	int likes;
	Post** timeline;

public:

	Page();
	void AddToTimeline(Post* post);
	char* GetID();
	char* GetName();

	void ReadPageFromFile(ifstream& fin);
	void PrintListView();
	void IncrementLikes();
	void ViewRecentPosts();
	void ViewTimeLine();							// DisplayPage

	~Page();
};

class Post
{
private:


	Activity* activity;


protected:

	char* ID;
	Date shareDate;
	char* text;
	Object* sharedBy;

	int likes;
	Object** likedBy;
	Comment** comments;

public:

	Post();
	Post(char* _ID, Date _shareDate, char* _text, Object* _sharedBy);
	char* GetID();
	Date GetDate();
	void ReadPostFromFile(ifstream& fin);
	void SetSharedBy(Object* object);
	void AlsoLikedBy(Object* object);
	void AddComment(Comment* _comment);
	virtual void Display(bool displayDate = true);
	void ViewLikedList();
	virtual ~Post();
};

class Memory : public Post
{
private:

	Post* originalPost;

public:

	Memory(Post* original, char* _ID, Date _shareDate, char* _text, Object* _sharedBy);
	void Display(bool displayDate = true);
	~Memory()
	{
		// calls destructor of base post
	}
};

class Activity
{
private:

	int type;
	char* value;

public:

	Activity();
	~Activity();
	void ReadActivity(ifstream& fin);
	void DisplayActivity();

};

class Comment
{
private:

	char* ID;
	char* text;

	Object* sharedBy;

public:

	Comment();
	Comment(char* _ID, char* _text, Object* _sharedBy);
	void ReadCommentFromFile(ifstream& fin);
	void DisplayComment();
	void SetSharedBy(Object* _sharedby);
	void SetText(char* _text);
	~Comment();
};

class Helper
{
public:

	static void StringCopy(const int& num, char* destination, char* source, const int& d_offset = 0, const int& s_offset = 0);

	static bool CompareString(char* str1, char* str2);

	static char* GetStringFromBuffer(char buffer[]);
	static char* StringConcatenate(char* str1, char* str2);
	static char* ConvertToString(int integer);

	static int StringLength(char* str);
};



/////////////////////////		 Main Function:		/////////////////////////

void main()
{
	Facebook fb;

	fb.LoadData("pages.txt", "users.txt", "posts.txt", "comments.txt");
	fb.Run("u7");

	system("pause");
}



/////////////////////////	Function Definitions:	/////////////////////////

/////// Member Functions of Class Date:

Date::Date(const int& _day, const int& _month, const int& _year)//
{
	day = _day;
	month = _month;
	year = _year;
}

void Date::SetDate(const int& _day, const int& _month, const int& _year)
{
	day = _day;
	month = _month;
	year = _year;
}
void Date::SetDate(const Date& rhs)
{
	day = rhs.day;
	month = rhs.month;
	year = rhs.year;
}

bool Date::CheckDateRange(const Date& rhs, const int& days)
{
	if (day > rhs.day - days)
		return true;
	else
		return false;
}
bool Date::IsDayMonthSame(const Date& rhs)
{
	if (day == rhs.day && month == rhs.month)
		return true;
	else
		return false;
}

int Date::YearDifference(const Date& rhs)
{
	return (year - rhs.year);
}


/////// Member Functions of Class Facebook:

Facebook::Facebook()
{
	currentUser = NULL;

	pages = NULL;
	users = NULL;
	posts = NULL;

	totalPages = 0;
	totalUsers = 0;
	totalPosts = 0;

	totalComments = 0;
	latestMemoryID = 0;
}
Facebook::~Facebook()
{
	if (pages != NULL)	// Deallocating all pages
		Deallocate<Page>(pages, totalPages);

	if (users != NULL)	// Deallocating all users
		Deallocate<User>(users, totalUsers);

	if (posts)
		delete[] posts;
}

Object* Facebook::SearchObjectByID(char* ID)
{
	if (ID[0] == 'p')
		return SearchPageByID(ID);
	else if (ID[0] == 'u')
		return SearchUserByID(ID);
	else
		return NULL;		// i.e. the ID represents neither a page nor a user
}
Page* Facebook::SearchPageByID(char* ID)
{
	for (int i = 0; i < totalPages; i++)
		if (Helper::CompareString(pages[i]->GetID(), ID))	// Checks whether IDs match
			return pages[i];
	return NULL; // i.e. page not found
}
User* Facebook::SearchUserByID(char* ID)
{
	for (int i = 0; i < totalUsers; i++)
		if (Helper::CompareString(users[i]->GetID(), ID))	// Checks whether IDs match
			return users[i];
	return NULL;	// i.e. user not found
}
Post* Facebook::SearchPostByID(char* ID)
{
	for (int i = 0; i < totalPosts; i++)
		if (Helper::CompareString(posts[i]->GetID(), ID))	// Checks whether IDs match
			return posts[i];
	return NULL;	// i.e. post not found
}

void Facebook::SetCurrentDate(int day, int month, int year)
{
	cout << "\nCommand:\tSet Current System Date: " << day << " " << month << " " << year << endl;
	Date::currentDate.SetDate(day, month, year);
	cout << "System Date:\t" << Date::currentDate << endl;
}
void Facebook::SetCurrentUser(char* user)
{
	currentUser = SearchUserByID(user);
	cout << "\nCommand:\tSet Current User: " << "\"" << currentUser->GetID() << "\"" << endl;
	cout << currentUser->GetName() << " successfully set as Current User!" << endl;
}

void Facebook::LoadData(char* pagefile, char* userfile, char* postfile, char* commentfile)
{
	LoadPages(pagefile);
	LoadUsers(userfile);
	LoadPosts(postfile);
	LoadComments(commentfile);
}
void Facebook::LoadPages(char* pagefile)
{
	ifstream fin;
	fin.open(pagefile);

	if (fin.is_open())
	{
		fin >> totalPages;

		if (totalPages > 0)		// to check whether no. of pages is a positive integer
		{
			pages = new Page*[totalPages];

			for (int i = 0; i < totalPages; i++)
			{
				pages[i] = new Page;
				pages[i]->ReadPageFromFile(fin);
			}
		}
		else
			cout << "Error: The total number of pages must be a positive integer!" << endl;

		fin.close();
	}
	else
		cout << "Error: The file 'pages.txt' could not be opened!" << endl;
}
void Facebook::LoadUsers(char* userfile)
{
	ifstream fin;
	fin.open(userfile);

	if (fin.is_open())
	{
		fin >> totalUsers;

		if (totalUsers > 0)	// to check whether no. of users is a positive integer
		{
			users = new User*[totalUsers];

			char*** tempFriends = new char**[totalUsers];	// for temporarily storing data regarding friends until all users have been loaded

			char buffer[BUFFER_SIZE] = { '\0' };

			for (int i = 0; i < totalUsers; i++)
			{
				users[i] = new User;
				users[i]->ReadUserFromFile(fin, tempFriends[i]);	// reads ID, first name, last name and friends, but not liked pages. They are read below:

				fin >> buffer;
				while (buffer[0] != '-')
				{
					char* tempID = Helper::GetStringFromBuffer(buffer);
					Page* tempPage = SearchPageByID(tempID);
					if (tempPage != NULL)
						users[i]->LikePage(tempPage);	// To associate liked pages with users

					fin >> buffer;
				}
			}

			for (int i = 0; i < totalUsers; i++)
				if (tempFriends[i] != NULL)
					for (int j = 0; tempFriends[i][j] != NULL; j++)
					{
						User* tempUser = SearchUserByID(tempFriends[i][j]);
						if (tempUser != NULL)
							users[i]->AddFriend(tempUser);	// To associate friends with users
					}

			// Deallocating temporary friends list:
			Deallocate(tempFriends, totalUsers);
		}
		else
			cout << "Error: The total number of users must be a positive integer!" << endl;

		fin.close();
	}
	else
		cout << "Error: The file 'users.txt' could not be opened!" << endl;
}
void Facebook::LoadPosts(char* postfile)
{
	ifstream fin;
	fin.open(postfile);

	if (fin.is_open())
	{
		fin >> totalPosts;

		if (totalPosts > 0)		// to check whether no. of posts is a positive integer
		{
			posts = new Post*[totalPosts];

			char buffer[BUFFER_SIZE] = { '\0' };
			fin.ignore();
			fin.getline(buffer, BUFFER_SIZE);	// to discard dotted line in text file

			for (int i = 0; i < totalPosts; i++)
			{
				posts[i] = new Post;
				posts[i]->ReadPostFromFile(fin);

				fin >> buffer;
				char* tempID = Helper::GetStringFromBuffer(buffer);

				Object* optr = SearchObjectByID(tempID);

				posts[i]->SetSharedBy(optr);
				optr->AddToTimeline(posts[i]);

				fin >> buffer;
				while (buffer[0] != '-')
				{
					char* tempID = Helper::GetStringFromBuffer(buffer);

					Object* tempObjPtr = SearchObjectByID(tempID);

					if (tempObjPtr != NULL)
						posts[i]->AlsoLikedBy(tempObjPtr);

					fin >> buffer;
				}

				fin.getline(buffer, BUFFER_SIZE);


			}
		}
		else
			cout << "Error: The total number of posts must be a positive integer!" << endl;

		fin.close();

		latestMemoryID = totalPosts;		// So that we are able to assign correct IDs to memories later on
	}
	else
		cout << "Error: The file 'posts.txt' could not be opened!" << endl;
}
void Facebook::LoadComments(char* commentfile)
{
	ifstream fin;
	fin.open(commentfile);

	if (fin.is_open())
	{
		fin >> totalComments;

		if (totalComments > 0)	// to check whether no. of comments is a positive integer
		{

			char buffer[BUFFER_SIZE] = { '\0' };

			for (int i = 0; i < totalComments; i++)
			{
				Comment* cPtr = new Comment;
				cPtr->ReadCommentFromFile(fin);

				fin >> buffer;
				char* tempPostID = Helper::GetStringFromBuffer(buffer);
				Post* tempPost = SearchPostByID(tempPostID);
				if (tempPost != NULL)
					tempPost->AddComment(cPtr);

				fin >> buffer;
				char* tempID = Helper::GetStringFromBuffer(buffer);
				Object* tempObj = SearchObjectByID(tempID);
				if (tempObj != NULL)
					cPtr->SetSharedBy(tempObj);

				fin.ignore();
				fin.getline(buffer, BUFFER_SIZE);
				cPtr->SetText(Helper::GetStringFromBuffer(buffer));
			}
		}
		else
			cout << "Error: The total number of comments must be a positive integer!" << endl;

		fin.close();
	}
	else
		cout << "Error: The file 'comments.txt' could not be opened!" << endl;
}

void Facebook::Run(char* user)
{
	SetCurrentDate(15, 11, 2017);
	SetCurrentUser(user);

	ViewFriendList();
	ViewLikedPages();

	ViewHomePage();
	ViewTimeLine();

	ViewLikedList("post5");
	LikePost("post5");
	ViewLikedList("post5");

	PostComment("post4", "Good Luck for your Result");
	ViewPost("post4");

	PostComment("post8", "Thanks for the wishes");
	ViewPost("post8");

	SeeYourMemories();
	ShareMemory("post10", "Never thought I will be specialist in this field...");
	ViewTimeLine();

	ViewPage("p1");
}

void Facebook::ViewFriendList()
{
	cout << "\nCommand:\tView Friends List" << endl;
	cout << "--------------------------------------------------" << endl;
	currentUser->ViewFriendsList();
}
void Facebook::ViewLikedPages()
{
	cout << "\nCommand:\tView Liked Pages" << endl;
	cout << "--------------------------------------------------" << endl;
	currentUser->ViewLikedPages();
}
void Facebook::ViewHomePage()
{
	cout << "\nCommand:\tView Home" << endl;
	cout << "--------------------------------------------------" << endl;
	cout << currentUser->GetfName() << " " << currentUser->GetlName() << " - Home Page" << endl << endl;
	currentUser->ViewHomePage();
}
void Facebook::ViewTimeLine()
{
	cout << "\nCommand:\tView Time Line" << endl;
	cout << "--------------------------------------------------" << endl;
	cout << currentUser->GetfName() << " " << currentUser->GetlName() << " - Time Line" << endl << endl;
	currentUser->ViewTimeLine();
}
void Facebook::SeeYourMemories()
{
	cout << "\nCommand:\tSeeYourMemories" << endl;
	currentUser->DisplayMemories();

	cout << "--------------------------------------------------" << endl;
}

void Facebook::LikePost(char* post)
{
	cout << "\nCommand:\tLikePost(" << post << ")" << endl;
	SearchPostByID(post)->AlsoLikedBy(currentUser);
}
void Facebook::ShareMemory(char* post, char* text)
{
	cout << "\nCommand:\tShareMemory(" << post << ", " << text << ")" << endl;
	char* newMemoryID = Helper::StringConcatenate("post", Helper::ConvertToString(++latestMemoryID));

	int textLength = Helper::StringLength(text);
	char* memoryText = new char[textLength + 1];
	Helper::StringCopy(textLength + 1, memoryText, text);
	
	Memory* memory = new Memory(SearchPostByID(post), newMemoryID, Date::currentDate, memoryText, currentUser);
	currentUser->AddToTimeline(memory);
}
void Facebook::PostComment(char* post, char* comment)
{
	cout << "\nCommand:\tPostComment(" << post << ", " << comment << ")" << endl;
	char* newCommentID = Helper::StringConcatenate("c", Helper::ConvertToString(++totalComments));

	int textLength = Helper::StringLength(comment);
	char* commentText = new char[textLength + 1];
	Helper::StringCopy(textLength + 1, commentText, comment);

	SearchPostByID(post)->AddComment(new Comment(newCommentID, commentText, currentUser));
}

void Facebook::ViewLikedList(char* post)
{
	cout << "\nCommand:\tViewLikedList(" << post << ")" << endl;
	cout << "--------------------------------------------------" << endl;
	SearchPostByID(post)->ViewLikedList();
	cout << "--------------------------------------------------" << endl;
}
void Facebook::ViewPost(char* post)
{
	cout << "\nCommand:\tViewPost(" << post << ")" << endl;
	SearchPostByID(post)->Display();
	cout << "--------------------------------------------------" << endl;
}

void Facebook::ViewPage(char* page)
{
	cout << "\nCommand:\tViewPage(" << page << ")" << endl;
	SearchPageByID(page)->ViewTimeLine();

	cout << "--------------------------------------------------" << endl;
}


/////// Member Functions of Class Object:

Object::Object()
{
	ID = NULL;
}
Object::~Object()
{
	if (ID)
		delete[] ID;
}


// Member Functions of Class User:

User::User()
{
	fName = NULL;
	lName = NULL;

	friendUsers = NULL;
	likedPages = NULL;

	timeline = NULL;
}
void User::AddToTimeline(Post* post)
{
	AddToList<Post>(timeline, post);
}
char* User::GetID()
{
	return ID;
}
char* User::GetfName()
{
	return fName;
}
char* User::GetlName()
{
	return lName;
}
void User::ReadUserFromFile(ifstream& fin, char**& tempFriends)
{
	char buffer[BUFFER_SIZE] = { '\0' };

	fin >> buffer;
	ID = Helper::GetStringFromBuffer(buffer);

	fin >> buffer;
	fName = Helper::GetStringFromBuffer(buffer);

	fin >> buffer;
	lName = Helper::GetStringFromBuffer(buffer);

	int friendsCount = 0;

	fin >> buffer;
	if (buffer[0] != '-')	// Loads list of friends into the temporary friend list
	{
		tempFriends = new char*[10];

		do
		{
			tempFriends[friendsCount++] = Helper::GetStringFromBuffer(buffer);
			fin >> buffer;
		} while (buffer[0] != '-');

		for (int i = friendsCount; i < 10; i++)
			tempFriends[i] = NULL;
	}
	else
		tempFriends = NULL;	// i.e. no friend
}
char* User::GetName()
{
	char* name = Helper::StringConcatenate(fName, " ");
	name = Helper::StringConcatenate(name, lName);
	return name;
}
void User::LikePage(Page*& page)	// To form association between users and liked pages
{
	if (likedPages == NULL)	// i.e. if first liked page
	{
		likedPages = new Page*[10];
		for (int i = 0; i < 10; i++)
			likedPages[i] = NULL;
	}

	for (int i = 0; i < 10; i++)
		if (likedPages[i] == NULL)	// First empty slot
		{
			likedPages[i] = page;
			page->IncrementLikes();
			return;
		}
}
void User::AddFriend(User*& user)	// To form association between users and friend users
{
	if (friendUsers == NULL)	// i.e. if first friend
	{
		friendUsers = new User*[10];
		for (int i = 0; i < 10; i++)
			friendUsers[i] = NULL;
	}

	for (int i = 0; i < 10; i++)
		if (friendUsers[i] == NULL)	// First empty slot
		{
			friendUsers[i] = user;
			return;
		}
}
void User::ViewFriendsList()
{
	cout << fName << " " << lName << " - Friends List:" << endl << endl;

	if (friendUsers != NULL)
		for (int i = 0; friendUsers[i] != NULL; i++)
			friendUsers[i]->PrintListView();
	else
		cout << "The currently selected user has no friends!" << endl;

	cout << "--------------------------------------------------" << endl;
}
void User::ViewLikedPages()
{
	cout << fName << " " << lName << " - Liked Pages:" << endl << endl;

	if (likedPages != NULL)
		for (int i = 0; likedPages[i] != NULL; i++)
			likedPages[i]->PrintListView();
	else
		cout << "The currently selected user has no liked pages!" << endl;

	cout << "--------------------------------------------------" << endl;
}
void User::PrintListView()
{
	cout << ID << " - " << fName << " " << lName << endl;
}
void User::ViewHomePage()
{
	if (friendUsers != NULL)
		for (int i = 0; friendUsers[i] != NULL; i++)
			friendUsers[i]->ViewRecentPosts();

	if (likedPages != NULL)
		for (int i = 0; likedPages[i] != NULL; i++)
			likedPages[i]->ViewRecentPosts();

	cout << "--------------------------------------------------" << endl;
}
void User::ViewRecentPosts()//
{
	if (timeline != NULL)
		for (int i = 0; timeline[i] != NULL; i++)
			if ((timeline[i]->GetDate()).CheckDateRange(Date::currentDate, 2))
				timeline[i]->Display(false);
}
void User::ViewTimeLine()
{
	if (timeline != NULL)
		for (int i = 0; timeline[i] != NULL; i++)
			timeline[i]->Display();

	cout << "--------------------------------------------------" << endl;
}
void User::DisplayMemories()
{
	cout << "We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago.\n" << endl;

	if (timeline != NULL)
		for (int i = 0; timeline[i] != NULL; i++)
			if ((timeline[i]->GetDate()).IsDayMonthSame(Date::currentDate))
			{
				cout << "On this Day,\n" << Date::currentDate.YearDifference(timeline[i]->GetDate()) << " Years Ago\n";
				timeline[i]->Display();
			}
}
User::~User()	// To deallocate all dynamically stored data of user class
{
	if (fName)
		delete[] fName;

	if (lName)
		delete[] lName;

	if (friendUsers)
		delete[] friendUsers;

	if (likedPages)
		delete[] likedPages;

	if (timeline)
		Deallocate<Post>(timeline);
}


// Member Functions of Class Page:

void Page::ReadPageFromFile(ifstream& fin)
{
	char buffer[BUFFER_SIZE] = { '\0' };

	fin >> buffer;
	ID = Helper::GetStringFromBuffer(buffer);

	fin.ignore();
	fin.getline(buffer, BUFFER_SIZE);	// Bcz title can have spaces in it
	title = Helper::GetStringFromBuffer(buffer);
}
void Page::PrintListView()
{
	cout << ID << " - " << title << endl;
}
Page::Page()
{
	title = NULL;
	likes = 0;
	Post* timeline = NULL;
}
void Page::AddToTimeline(Post* post)
{
	AddToList<Post>(timeline, post);
}
char* Page::GetID()
{
	return ID;
}
char* Page::GetName()//
{
	return title;
}
void Page::IncrementLikes()
{
	likes++;
}
Page::~Page()	// To deallocate all dynamically allocated data of page class
{
	if (title)
		delete[] title;

	if (timeline)
		Deallocate<Post>(timeline);
}
void Page::ViewRecentPosts()//
{
	if (timeline != NULL)
		for (int i = 0; timeline[i] != NULL; i++)
			if ((timeline[i]->GetDate()).CheckDateRange(Date::currentDate, 2))
				timeline[i]->Display(false);
}
void Page::ViewTimeLine()
{
	if (timeline != NULL)
		for (int i = 0; timeline[i] != NULL; i++)
			timeline[i]->Display();
}


// Member Functions of Class Post:

Post::Post()//
{
	ID = NULL;
	activity = NULL;
	text = NULL;
	sharedBy = NULL;
	likes = 0;
	likedBy = NULL;
	comments = NULL;
}
Post::Post(char* _ID, Date _shareDate, char* _text, Object* _sharedBy)
{
	ID = _ID;
	activity = NULL;
	text = _text;
	sharedBy = _sharedBy;
	likedBy = NULL;
	comments = NULL;

	shareDate.SetDate(_shareDate);
}
char* Post::GetID()
{
	return ID;
}
Date Post::GetDate()
{
	return shareDate;//
}
void Post::ReadPostFromFile(ifstream& fin)
{
	char buffer[BUFFER_SIZE] = { '\0' };

	int postType = 1;
	fin >> postType;

	if (postType == 2)
		activity = new Activity;

	fin >> buffer;
	ID = Helper::GetStringFromBuffer(buffer);

	int day = 0, month = 0, year = 0;
	fin >> day >> month >> year;
	shareDate.SetDate(day, month, year);

	fin.ignore();//
	fin.getline(buffer, BUFFER_SIZE);
	text = Helper::GetStringFromBuffer(buffer);

	if (activity != NULL)
		activity->ReadActivity(fin);
}
void Post::SetSharedBy(Object* object)
{
	sharedBy = object;
}
void Post::AlsoLikedBy(Object* object)
{
	if (likedBy == NULL)	// i.e. if first liking
	{
		likedBy = new Object*[10];
		for (int i = 0; i < 10; i++)
			likedBy[i] = NULL;
	}

	for (int i = 0; i < 10; i++)
		if (likedBy[i] == NULL)	// First empty slot
		{
			likedBy[i] = object;
			likes++;
			return;
		}
}
void Post::AddComment(Comment* _comment)
{
	AddToList<Comment>(comments, _comment);
}
void Post::Display(bool displayDate)//
{
	cout << "---" << sharedBy->GetName();
	if (activity != NULL)
		activity->DisplayActivity();
	else
		cout << " shared ";

	cout << "\"" << text << "\"";

	if (displayDate == true)
		cout << " ... " << "(" << shareDate << ")";

	cout << endl;

	if (comments != NULL)
		for (int i = 0; comments[i] != NULL; i++)
			comments[i]->DisplayComment();

	cout << endl;
}
void Post::ViewLikedList()
{
	cout << "Post Liked By:" << endl;

	if (likedBy != NULL)
		for (int i = 0; likedBy[i] != NULL; i++)
			likedBy[i]->PrintListView();
}
Post::~Post()
{
	if (ID)
		delete[] ID;

	if (activity)
		delete activity;

	if (text)
		delete[] text;
	
	if (likedBy)
		delete[] likedBy;

	if (comments)
		Deallocate<Comment>(comments);
}


// Member Functions of Class Memory:

Memory::Memory(Post* original, char* _ID, Date _shareDate, char* _text, Object* _sharedBy) : Post(_ID, _shareDate, _text, _sharedBy)
{
	originalPost = original;
}
void Memory::Display(bool displayDate)
{
	cout << "~~~" << sharedBy->GetName() << " shared a memory~~~";

	if (displayDate == true)
		cout << " ... " << "(" << shareDate << ")";

	cout << "\n\"" << text << "\"";

	cout << "\n\t\t(" << shareDate.YearDifference(originalPost->GetDate()) << " Years Ago)\n";

	originalPost->Display();

	cout << endl;
}


// Member Functions of Class Activity:

Activity::Activity()
{
	type = 0;
	value = NULL;
}
Activity::~Activity()
{
	if (value)
		delete[] value;
}
void Activity::ReadActivity(ifstream& fin)
{
	fin >> type;

	char buffer[BUFFER_SIZE] = { '\0' };

	fin.ignore();
	fin.getline(buffer, BUFFER_SIZE);
	value = Helper::GetStringFromBuffer(buffer);
}
void Activity::DisplayActivity()
{
	cout << " is ";

	if (type == 1)
		cout << "feeling ";
	else if (type == 2)
		cout << "thinking ";
	else if (type == 3)
		cout << "making ";
	else if (type == 4)
		cout << "celebrating ";

	cout << value << endl;
}


// Member Functions of Class Comment:

void Comment::ReadCommentFromFile(ifstream& fin)
{
	char buffer[BUFFER_SIZE] = { '\0' };

	fin >> buffer;
	ID = Helper::GetStringFromBuffer(buffer);
}
void Comment::DisplayComment()
{
	cout << sharedBy->GetName() << " wrote: \"" << text << "\"" << endl;
}
Comment::Comment()
{
	ID = NULL;
	text = NULL;
	sharedBy = NULL;
}
Comment::Comment(char* _ID, char* _text, Object* _sharedBy)//////
{
	ID = _ID;
	text = _text;
	sharedBy = _sharedBy;
}
void Comment::SetSharedBy(Object* _sharedby) // or object?
{
	sharedBy = _sharedby;
}
void Comment::SetText(char* _text)
{
	text = _text;
}
Comment::~Comment()
{
	if (ID)
		delete[] ID;

	if (text)
		delete[] text;
}


/////// Member Functions of Class Helper:

void Helper::StringCopy(const int& num, char* destination, char* source, const int& d_offset, const int& s_offset)
{
	// Assumes that there is sufficient space in the destination string
	// Copies num chars from source string to destination string
	// With offset feature for both source and destination

	for (int i = 0; i < num; i++)
		destination[d_offset + i] = source[s_offset + i];
}

bool Helper::CompareString(char* str1, char* str2)
{
	int length = StringLength(str1);
	if (length == StringLength(str2))
	{
		for (int i = 0; i < length; i++)
			if (str1[i] != str2[i])		// If any corresponding character does not match, the strings are not equal.
				return false;

		return true;
	}
	else     // If the lengths of two strings are not equal, they cannot be equal.
		return false;
}

char* Helper::GetStringFromBuffer(char buffer[])
{
	int length = StringLength(buffer);

	char* str = new char[length + 1];	// +1 in order to have space for null character as well at the end.
	StringCopy(length + 1, str, buffer);	// length+1 copies null character as well.

	return str;
}
char* Helper::StringConcatenate(char* str1, char* str2)
{
	int len1 = StringLength(str1);
	int len2 = StringLength(str2);
	//
	char* temp = new char[len1 + len2 + 1];			// 1 extra for null character

	StringCopy(len1, temp, str1, 0, 0);
	StringCopy(len2 + 1, temp, str2, len1, 0);		// len2 + 1 to copy null char as well; len1 is destination offset
	// See COMMENT ON DEFINITION OF STRINGCOPY for details
	return temp;
}
char* Helper::ConvertToString(int integer)//
{

	char* str = NULL;

	if (integer > 0)
	{
		int limit = 10;
		int digits = 1;
		while (integer >= limit)
		{
			limit *= 10;
			digits++;
		}
		str = new char[digits + 1];

		for (int i = 0; i < digits; i++)
		{
			str[digits - 1 - i] = '0' + integer % 10;
			integer /= 10;
		}

		str[digits] = '\0';
	}

	return str;
}

int Helper::StringLength(char* str)
{
	int length = 0;
	while (str[length] != '\0')
		length++;
	return length;
}


/////////////////////////	Overloaded Operators:	/////////////////////////

ostream& operator<<(ostream& out, const Date& date)
{
	out << date.day << "/" << date.month << "/" << date.year;
	return out;
}