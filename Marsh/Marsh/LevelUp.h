#pragma once
#include "Main.h"
#include <iostream>
#define STAT_BOOST 5
using namespace std;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace Marsh {


	typedef struct AttackNode{
		Attack* attack;
		string name;
		string description;
		int index;
		int previousAttack;
	}AttackNode;

	/// <summary>
	/// Summary for LevelUp
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class LevelUp : public System::Windows::Forms::Form
	{
	public:
		Player* hero;
		std::vector<AttackNode*>* nodes;
		Marsh::AttackNode* currentNode;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::PictureBox^  pictureBox4;
	private: System::Windows::Forms::PictureBox^  pictureBox5;
	private: System::Windows::Forms::PictureBox^  pictureBox6;
	private: System::Windows::Forms::PictureBox^  pictureBox7;
	private: System::Windows::Forms::PictureBox^  pictureBox8;
	private: System::Windows::Forms::PictureBox^  pictureBox9;
	private: System::Windows::Forms::PictureBox^  pictureBox10;
	private: System::Windows::Forms::PictureBox^  pictureBox11;
	private: System::Windows::Forms::PictureBox^  pictureBox12;
	private: System::Windows::Forms::RichTextBox^  SpellInfo;
	public: 

	public: 

	public: 











	private: System::Windows::Forms::Label^  SpellPointsLabel;
	public: 
	
		LevelUp(Player* hero)
		{
			ShowWindow(static_cast<HWND>(Handle.ToPointer()) ,SW_SHOW);
			this->Visible = false;
			this->TopMost = true;
			InitializeComponent();

			if(hero == NULL){
				return;
			}
			nodes = new std::vector<AttackNode*>();
			this->hero = hero;
			this->SpellPointsLabel->Text = "You can get " + hero->spellPoints + " new spells.";
			this->StatPointsLabel->Text = "Current Stat Points: " + hero->statPoints;
			this->IntelligenceLabel->Text = "Intelligence: " + hero->intelligence;
			this->WillpowerLabel->Text = "Willpower: " + hero->willpower;
			this->FocusLabel->Text = "Focus: " + hero->focus;
			this->VitalityLabel->Text = "Vitality: " + hero->vitality;
			this->BringToFront();
			createNodes();
			disableGottenSpells();

			/*for(int i = 0; i < 15; i++){
			Attack* attack = new Attack(800, 800, 2, 10, new Attack_Sprite("Resources//magic//fireball.bmp", W, 5, 1, 5, 5, 26,26), 51,0,MID_RANGE, 0,0,10);
			attack->my_type = Wallop;
			attack->set_boundary_value(26, 26, 2, 2);
			attack->set_my_caster(hero);


			std::string s;
			std::stringstream out;
			out << i;
			s = out.str();

			AttackNode* node = new AttackNode();
			node->name = "Fireball " + s;
			node->description = "This is a fireball number " + s;
			node->index = i;
			node->attack = attack;
			node->previousAttack = NULL;
			nodes->push_back(node);
			}*/

			currentNode = NULL;
		}

		void createNodes(){
			AttackNode* node;

			node = new AttackNode();
			node->name = "Shadow Ball";
			node->description = "This is a long range starting attack that is a ball made of shadows.";
			node->index = SHADOW_BALL;
			node->attack = attackDB->fetch_attack(SHADOW_BALL);
			node->attack->my_caster = hero;
			node->previousAttack = -1;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Shadow Ball +";
			node->description = "This is an upgrade on the shadow ball attack.";
			node->index = SHADOW_BALL_PLUS;
			node->attack = attackDB->fetch_attack(SHADOW_BALL_PLUS);
			node->attack->my_caster = hero;
			node->previousAttack = SHADOW_BALL;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Shadow Wave";
			node->description = "This is a short range attack of waves of shadows.";
			node->index = SHADOW_WAVE;
			node->attack = attackDB->fetch_attack(SHADOW_WAVE);
			node->attack->my_caster = hero;
			node->previousAttack = SHADOW_BALL;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Shadow Nova ";
			node->description = "This massive attack that traps the enemy and attacks them.";
			node->index = SHADOW_NOVA;
			node->attack = attackDB->fetch_attack(SHADOW_NOVA);
			node->attack->my_caster = hero;
			node->previousAttack = SHADOW_WAVE;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Shadow Needle";
			node->description = "This is a long range starting attack needle made of shadows.";
			node->index = SHADOW_NEEDLE;
			node->attack = attackDB->fetch_attack(SHADOW_NEEDLE);
			node->attack->my_caster = hero;
			node->previousAttack = -1;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Shadow Needle +";
			node->description = "This is an upgrade to the shadow needle attack.";
			node->index = SHADOW_NEEDLE_PLUS;
			node->attack = attackDB->fetch_attack(SHADOW_NEEDLE_PLUS);
			node->attack->my_caster = hero;
			node->previousAttack = SHADOW_NEEDLE;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Shadow Spikes";
			node->description = "This is a short range attack where spikes made of shadows come out of the ground.";
			node->index = SHADOW_SPIKES;
			node->attack = attackDB->fetch_attack(SHADOW_SPIKES);
			node->attack->my_caster = hero;
			node->previousAttack = SHADOW_NEEDLE;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Death Beam";
			node->description = "This is a long barrage of death.";
			node->index = DEATH_BEAM;
			node->attack = attackDB->fetch_attack(DEATH_BEAM);
			node->attack->my_caster = hero;
			node->previousAttack = SHADOW_SPIKES;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Drain Health";
			node->description = "This is a standard attack that attacks the enemy and heals you.";
			node->index = DRAIN_HEALTH;
			node->attack = attackDB->fetch_attack(DRAIN_HEALTH);
			node->attack->my_caster = hero;
			node->previousAttack = -1;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Bind in Shadows";
			node->description = "This is binding attack made of shadows that stops the enemy.";
			node->index = BIND_IN_SHADOWS;
			node->attack = attackDB->fetch_attack(BIND_IN_SHADOWS);
			node->attack->my_caster = hero;
			node->previousAttack = DRAIN_HEALTH;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Mass Slow";
			node->description = "This is a slowing attack that makes enemies really slow.";
			node->index = MASS_SLOW;
			node->attack = attackDB->fetch_attack(MASS_SLOW);
			node->attack->my_caster = hero;
			node->previousAttack = BIND_IN_SHADOWS;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Shadow Dash";
			node->description = "This is a quick dash.";
			node->index = SHADOW_DASH;
			node->attack = attackDB->fetch_attack(SHADOW_DASH);
			node->attack->my_caster = hero;
			node->previousAttack = DRAIN_HEALTH;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Teleport";
			node->description = "This is longer teleport for the player.";
			node->index = TELEPORT;
			node->attack = attackDB->fetch_attack(TELEPORT);
			node->attack->my_caster = hero;
			node->previousAttack = SHADOW_DASH;
			nodes->push_back(node);

			//node = new AttackNode();
			//node->name = "Shadow Steps";
			//node->description = "A quick succession of teleports.";
			//node->index = 13;
			//node->attack = attackDB->fetch_attack(13);
			//node->attack->my_caster = hero;
			//node->previousAttack = TELEPORT;
			//nodes->push_back(node);

			node = new AttackNode();
			node->name = "Shield";
			node->description = "This is a protective shield that stops one attack.";
			node->index = SHIELD;
			node->attack = attackDB->fetch_attack(SHIELD);
			node->attack->my_caster = hero;
			node->previousAttack = DRAIN_HEALTH;
			nodes->push_back(node);

			node = new AttackNode();
			node->name = "Deflect";
			node->description = "This is a stronger shield.";
			node->index = DEFLECT;
			node->attack = attackDB->fetch_attack(DEFLECT);
			node->attack->my_caster = hero;
			node->previousAttack = SHIELD;
			nodes->push_back(node);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LevelUp()
		{
			if (components)
			{
				delete components;
			}
		}

		int findLoadoutIndex(){
			for(int i = 0; i < MAX_ATTACKS; i++){
				try{
					if(hero->attack_loadout[i]->range == NULL){
						return i;
					}
				}
				catch(...){
					return i;//TODO make this work without exceptions?
				}
			}
			return -1;
		}

		void decreaseStatPoints(void){
			hero->statPoints--;
			this->StatPointsLabel->Text = "Current Stat Points: " + hero->statPoints;
		}
	private: System::Windows::Forms::Button^  Spell0;
	private: System::Windows::Forms::Button^  Spell1;
	private: System::Windows::Forms::Button^  Spell2;
	private: System::Windows::Forms::Button^  Spell3;
	private: System::Windows::Forms::Button^  Spell4;
	private: System::Windows::Forms::Button^  Spell5;
	private: System::Windows::Forms::Button^  Spell6;
	private: System::Windows::Forms::Button^  Spell7;
	private: System::Windows::Forms::Button^  Spell8;
	private: System::Windows::Forms::Button^  Spell9;
	private: System::Windows::Forms::Button^  Spell10;
	private: System::Windows::Forms::Button^  Spell11;
	private: System::Windows::Forms::Button^  Spell12;
	private: System::Windows::Forms::Button^  Spell13;
	private: System::Windows::Forms::Button^  Spell14;
	

	private: System::Windows::Forms::Button^  AcceptSpellButton;
	private: System::Windows::Forms::Button^  ReturnButton;
	private: System::Windows::Forms::Label^  StatPointsLabel;
	private: System::Windows::Forms::Label^  IntelligenceLabel;
	private: System::Windows::Forms::Label^  WillpowerLabel;
	private: System::Windows::Forms::Label^  FocusLabel;
	private: System::Windows::Forms::Label^  VitalityLabel;
	private: System::Windows::Forms::Button^  IntelligenceButton;
	private: System::Windows::Forms::Button^  WillpowerButton;
	private: System::Windows::Forms::Button^  focusButton;
	private: System::Windows::Forms::Button^  VitalityButton;


			 /*protected:
			 virtual void OnLoad(EventArgs^ e)override{
			 if(hero == NULL){
			 Close();
			 }
			 }*/

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(LevelUp::typeid));
			this->Spell0 = (gcnew System::Windows::Forms::Button());
			this->Spell1 = (gcnew System::Windows::Forms::Button());
			this->Spell2 = (gcnew System::Windows::Forms::Button());
			this->Spell3 = (gcnew System::Windows::Forms::Button());
			this->Spell4 = (gcnew System::Windows::Forms::Button());
			this->Spell5 = (gcnew System::Windows::Forms::Button());
			this->Spell6 = (gcnew System::Windows::Forms::Button());
			this->Spell7 = (gcnew System::Windows::Forms::Button());
			this->Spell8 = (gcnew System::Windows::Forms::Button());
			this->Spell9 = (gcnew System::Windows::Forms::Button());
			this->Spell10 = (gcnew System::Windows::Forms::Button());
			this->Spell11 = (gcnew System::Windows::Forms::Button());
			this->Spell12 = (gcnew System::Windows::Forms::Button());
			this->Spell13 = (gcnew System::Windows::Forms::Button());
			this->Spell14 = (gcnew System::Windows::Forms::Button());
			this->AcceptSpellButton = (gcnew System::Windows::Forms::Button());
			this->ReturnButton = (gcnew System::Windows::Forms::Button());
			this->StatPointsLabel = (gcnew System::Windows::Forms::Label());
			this->IntelligenceLabel = (gcnew System::Windows::Forms::Label());
			this->WillpowerLabel = (gcnew System::Windows::Forms::Label());
			this->FocusLabel = (gcnew System::Windows::Forms::Label());
			this->VitalityLabel = (gcnew System::Windows::Forms::Label());
			this->IntelligenceButton = (gcnew System::Windows::Forms::Button());
			this->WillpowerButton = (gcnew System::Windows::Forms::Button());
			this->focusButton = (gcnew System::Windows::Forms::Button());
			this->VitalityButton = (gcnew System::Windows::Forms::Button());
			this->SpellPointsLabel = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox5 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox6 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox7 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox8 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox9 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox10 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox11 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox12 = (gcnew System::Windows::Forms::PictureBox());
			this->SpellInfo = (gcnew System::Windows::Forms::RichTextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox8))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox9))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox10))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox11))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox12))->BeginInit();
			this->SuspendLayout();
			// 
			// Spell0
			// 
			this->Spell0->BackColor = System::Drawing::Color::Black;
			this->Spell0->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell0->FlatAppearance->BorderSize = 2;
			this->Spell0->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell0->ForeColor = System::Drawing::Color::DarkRed;
			this->Spell0->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell0.Image")));
			this->Spell0->Location = System::Drawing::Point(99, 42);
			this->Spell0->Name = L"Spell0";
			this->Spell0->Size = System::Drawing::Size(59, 58);
			this->Spell0->TabIndex = 0;
			this->Spell0->Tag = L"0";
			this->Spell0->UseVisualStyleBackColor = false;
			this->Spell0->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell1
			// 
			this->Spell1->BackColor = System::Drawing::Color::Black;
			this->Spell1->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell1->FlatAppearance->BorderSize = 2;
			this->Spell1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell1->ForeColor = System::Drawing::Color::Transparent;
			this->Spell1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell1.Image")));
			this->Spell1->Location = System::Drawing::Point(280, 140);
			this->Spell1->Name = L"Spell1";
			this->Spell1->Size = System::Drawing::Size(59, 58);
			this->Spell1->TabIndex = 2;
			this->Spell1->Tag = L"1";
			this->Spell1->UseVisualStyleBackColor = false;
			this->Spell1->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell2
			// 
			this->Spell2->BackColor = System::Drawing::Color::Black;
			this->Spell2->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell2->FlatAppearance->BorderSize = 2;
			this->Spell2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell2->ForeColor = System::Drawing::Color::Transparent;
			this->Spell2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell2.Image")));
			this->Spell2->Location = System::Drawing::Point(280, 42);
			this->Spell2->Name = L"Spell2";
			this->Spell2->Size = System::Drawing::Size(59, 58);
			this->Spell2->TabIndex = 1;
			this->Spell2->Tag = L"2";
			this->Spell2->UseVisualStyleBackColor = false;
			this->Spell2->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell3
			// 
			this->Spell3->BackColor = System::Drawing::Color::Transparent;
			this->Spell3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell3.BackgroundImage")));
			this->Spell3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->Spell3->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell3->FlatAppearance->BorderSize = 2;
			this->Spell3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell3->ForeColor = System::Drawing::Color::Transparent;
			this->Spell3->Location = System::Drawing::Point(461, 42);
			this->Spell3->Name = L"Spell3";
			this->Spell3->Size = System::Drawing::Size(59, 58);
			this->Spell3->TabIndex = 3;
			this->Spell3->Tag = L"3";
			this->Spell3->UseVisualStyleBackColor = false;
			this->Spell3->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell4
			// 
			this->Spell4->BackColor = System::Drawing::Color::Transparent;
			this->Spell4->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell4.BackgroundImage")));
			this->Spell4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->Spell4->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell4->FlatAppearance->BorderSize = 2;
			this->Spell4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell4->ForeColor = System::Drawing::Color::Transparent;
			this->Spell4->Location = System::Drawing::Point(99, 276);
			this->Spell4->Name = L"Spell4";
			this->Spell4->Size = System::Drawing::Size(59, 58);
			this->Spell4->TabIndex = 4;
			this->Spell4->Tag = L"4";
			this->Spell4->UseVisualStyleBackColor = false;
			this->Spell4->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell5
			// 
			this->Spell5->BackColor = System::Drawing::Color::Black;
			this->Spell5->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell5.BackgroundImage")));
			this->Spell5->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->Spell5->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell5->FlatAppearance->BorderSize = 2;
			this->Spell5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell5->ForeColor = System::Drawing::Color::Transparent;
			this->Spell5->Location = System::Drawing::Point(280, 371);
			this->Spell5->Name = L"Spell5";
			this->Spell5->Size = System::Drawing::Size(59, 58);
			this->Spell5->TabIndex = 6;
			this->Spell5->Tag = L"5";
			this->Spell5->UseVisualStyleBackColor = false;
			this->Spell5->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell6
			// 
			this->Spell6->BackColor = System::Drawing::Color::Black;
			this->Spell6->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell6->FlatAppearance->BorderSize = 2;
			this->Spell6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell6->ForeColor = System::Drawing::Color::Transparent;
			this->Spell6->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell6.Image")));
			this->Spell6->Location = System::Drawing::Point(280, 276);
			this->Spell6->Name = L"Spell6";
			this->Spell6->Size = System::Drawing::Size(59, 58);
			this->Spell6->TabIndex = 5;
			this->Spell6->Tag = L"6";
			this->Spell6->UseVisualStyleBackColor = false;
			this->Spell6->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell7
			// 
			this->Spell7->BackColor = System::Drawing::Color::Black;
			this->Spell7->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell7.BackgroundImage")));
			this->Spell7->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->Spell7->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell7->FlatAppearance->BorderSize = 2;
			this->Spell7->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell7->ForeColor = System::Drawing::Color::Transparent;
			this->Spell7->Location = System::Drawing::Point(461, 276);
			this->Spell7->Name = L"Spell7";
			this->Spell7->Size = System::Drawing::Size(59, 58);
			this->Spell7->TabIndex = 7;
			this->Spell7->Tag = L"7";
			this->Spell7->UseVisualStyleBackColor = false;
			this->Spell7->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell8
			// 
			this->Spell8->BackColor = System::Drawing::Color::Transparent;
			this->Spell8->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell8.BackgroundImage")));
			this->Spell8->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->Spell8->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell8->FlatAppearance->BorderSize = 2;
			this->Spell8->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell8->ForeColor = System::Drawing::Color::Transparent;
			this->Spell8->Location = System::Drawing::Point(99, 624);
			this->Spell8->Name = L"Spell8";
			this->Spell8->Size = System::Drawing::Size(59, 58);
			this->Spell8->TabIndex = 8;
			this->Spell8->Tag = L"8";
			this->Spell8->UseVisualStyleBackColor = false;
			this->Spell8->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell9
			// 
			this->Spell9->BackColor = System::Drawing::Color::Transparent;
			this->Spell9->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell9.BackgroundImage")));
			this->Spell9->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->Spell9->FlatAppearance->BorderColor = System::Drawing::Color::Black;
			this->Spell9->FlatAppearance->BorderSize = 2;
			this->Spell9->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell9->ForeColor = System::Drawing::Color::Transparent;
			this->Spell9->Location = System::Drawing::Point(280, 482);
			this->Spell9->Name = L"Spell9";
			this->Spell9->Size = System::Drawing::Size(59, 58);
			this->Spell9->TabIndex = 9;
			this->Spell9->Tag = L"9";
			this->Spell9->UseVisualStyleBackColor = false;
			this->Spell9->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell10
			// 
			this->Spell10->BackColor = System::Drawing::Color::Transparent;
			this->Spell10->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell10.BackgroundImage")));
			this->Spell10->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->Spell10->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell10->FlatAppearance->BorderSize = 2;
			this->Spell10->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell10->ForeColor = System::Drawing::Color::Transparent;
			this->Spell10->Location = System::Drawing::Point(461, 482);
			this->Spell10->Name = L"Spell10";
			this->Spell10->Size = System::Drawing::Size(59, 58);
			this->Spell10->TabIndex = 11;
			this->Spell10->Tag = L"10";
			this->Spell10->UseVisualStyleBackColor = false;
			this->Spell10->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell11
			// 
			this->Spell11->BackColor = System::Drawing::Color::Transparent;
			this->Spell11->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell11.BackgroundImage")));
			this->Spell11->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->Spell11->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell11->FlatAppearance->BorderSize = 2;
			this->Spell11->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell11->ForeColor = System::Drawing::Color::Transparent;
			this->Spell11->Location = System::Drawing::Point(280, 624);
			this->Spell11->Name = L"Spell11";
			this->Spell11->Size = System::Drawing::Size(59, 58);
			this->Spell11->TabIndex = 14;
			this->Spell11->Tag = L"11";
			this->Spell11->UseVisualStyleBackColor = false;
			this->Spell11->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell12
			// 
			this->Spell12->BackColor = System::Drawing::Color::Transparent;
			this->Spell12->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell12.BackgroundImage")));
			this->Spell12->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->Spell12->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell12->FlatAppearance->BorderSize = 2;
			this->Spell12->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell12->ForeColor = System::Drawing::Color::Transparent;
			this->Spell12->Location = System::Drawing::Point(461, 624);
			this->Spell12->Name = L"Spell12";
			this->Spell12->Size = System::Drawing::Size(59, 58);
			this->Spell12->TabIndex = 12;
			this->Spell12->Tag = L"12";
			this->Spell12->UseVisualStyleBackColor = false;
			this->Spell12->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell13
			// 
			this->Spell13->BackColor = System::Drawing::Color::Transparent;
			this->Spell13->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell13.BackgroundImage")));
			this->Spell13->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->Spell13->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell13->FlatAppearance->BorderSize = 2;
			this->Spell13->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell13->ForeColor = System::Drawing::Color::Transparent;
			this->Spell13->Location = System::Drawing::Point(280, 756);
			this->Spell13->Name = L"Spell13";
			this->Spell13->Size = System::Drawing::Size(59, 58);
			this->Spell13->TabIndex = 13;
			this->Spell13->Tag = L"13";
			this->Spell13->UseVisualStyleBackColor = false;
			this->Spell13->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// Spell14
			// 
			this->Spell14->BackColor = System::Drawing::Color::Transparent;
			this->Spell14->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Spell14.BackgroundImage")));
			this->Spell14->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->Spell14->FlatAppearance->BorderColor = System::Drawing::SystemColors::ControlText;
			this->Spell14->FlatAppearance->BorderSize = 2;
			this->Spell14->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Spell14->ForeColor = System::Drawing::Color::Transparent;
			this->Spell14->Location = System::Drawing::Point(461, 756);
			this->Spell14->Name = L"Spell14";
			this->Spell14->Size = System::Drawing::Size(59, 58);
			this->Spell14->TabIndex = 14;
			this->Spell14->Tag = L"14";
			this->Spell14->UseVisualStyleBackColor = false;
			this->Spell14->Click += gcnew System::EventHandler(this, &LevelUp::Spell_Click);
			// 
			// AcceptSpellButton
			// 
			this->AcceptSpellButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->AcceptSpellButton->Font = (gcnew System::Drawing::Font(L"Segoe Print", 18));
			this->AcceptSpellButton->Location = System::Drawing::Point(753, 653);
			this->AcceptSpellButton->Name = L"AcceptSpellButton";
			this->AcceptSpellButton->Size = System::Drawing::Size(181, 103);
			this->AcceptSpellButton->TabIndex = 16;
			this->AcceptSpellButton->Text = L"Accept New Spell";
			this->AcceptSpellButton->UseVisualStyleBackColor = true;
			this->AcceptSpellButton->Click += gcnew System::EventHandler(this, &LevelUp::AcceptSpellButton_Click);
			// 
			// ReturnButton
			// 
			this->ReturnButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ReturnButton->Font = (gcnew System::Drawing::Font(L"Segoe Print", 18));
			this->ReturnButton->Location = System::Drawing::Point(953, 653);
			this->ReturnButton->Name = L"ReturnButton";
			this->ReturnButton->Size = System::Drawing::Size(164, 103);
			this->ReturnButton->TabIndex = 17;
			this->ReturnButton->Text = L"Return";
			this->ReturnButton->UseVisualStyleBackColor = true;
			this->ReturnButton->Click += gcnew System::EventHandler(this, &LevelUp::ReturnButton_Click);
			// 
			// StatPointsLabel
			// 
			this->StatPointsLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->StatPointsLabel->AutoSize = true;
			this->StatPointsLabel->Font = (gcnew System::Drawing::Font(L"Segoe Print", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->StatPointsLabel->Location = System::Drawing::Point(710, 42);
			this->StatPointsLabel->Name = L"StatPointsLabel";
			this->StatPointsLabel->Size = System::Drawing::Size(276, 42);
			this->StatPointsLabel->TabIndex = 20;
			this->StatPointsLabel->Text = L"Current Stat Points: ";
			// 
			// IntelligenceLabel
			// 
			this->IntelligenceLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->IntelligenceLabel->AutoSize = true;
			this->IntelligenceLabel->Font = (gcnew System::Drawing::Font(L"Segoe Print", 18));
			this->IntelligenceLabel->Location = System::Drawing::Point(807, 126);
			this->IntelligenceLabel->Name = L"IntelligenceLabel";
			this->IntelligenceLabel->Size = System::Drawing::Size(159, 42);
			this->IntelligenceLabel->TabIndex = 20;
			this->IntelligenceLabel->Text = L"Intelligence:";
			// 
			// WillpowerLabel
			// 
			this->WillpowerLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->WillpowerLabel->AutoSize = true;
			this->WillpowerLabel->Font = (gcnew System::Drawing::Font(L"Segoe Print", 18));
			this->WillpowerLabel->Location = System::Drawing::Point(807, 167);
			this->WillpowerLabel->Name = L"WillpowerLabel";
			this->WillpowerLabel->Size = System::Drawing::Size(145, 42);
			this->WillpowerLabel->TabIndex = 19;
			this->WillpowerLabel->Text = L"Willpower:";
			// 
			// FocusLabel
			// 
			this->FocusLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->FocusLabel->AutoSize = true;
			this->FocusLabel->Font = (gcnew System::Drawing::Font(L"Segoe Print", 18));
			this->FocusLabel->Location = System::Drawing::Point(807, 212);
			this->FocusLabel->Name = L"FocusLabel";
			this->FocusLabel->Size = System::Drawing::Size(100, 42);
			this->FocusLabel->TabIndex = 20;
			this->FocusLabel->Text = L"Focus: ";
			// 
			// VitalityLabel
			// 
			this->VitalityLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->VitalityLabel->AutoSize = true;
			this->VitalityLabel->Font = (gcnew System::Drawing::Font(L"Segoe Print", 18));
			this->VitalityLabel->Location = System::Drawing::Point(807, 83);
			this->VitalityLabel->Name = L"VitalityLabel";
			this->VitalityLabel->Size = System::Drawing::Size(113, 42);
			this->VitalityLabel->TabIndex = 21;
			this->VitalityLabel->Text = L"Vitality:";
			// 
			// IntelligenceButton
			// 
			this->IntelligenceButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->IntelligenceButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->IntelligenceButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->IntelligenceButton->Location = System::Drawing::Point(713, 131);
			this->IntelligenceButton->Name = L"IntelligenceButton";
			this->IntelligenceButton->Size = System::Drawing::Size(88, 33);
			this->IntelligenceButton->TabIndex = 22;
			this->IntelligenceButton->Text = L"+";
			this->IntelligenceButton->UseVisualStyleBackColor = true;
			this->IntelligenceButton->Click += gcnew System::EventHandler(this, &LevelUp::IntelligenceButton_Click);
			// 
			// WillpowerButton
			// 
			this->WillpowerButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->WillpowerButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->WillpowerButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->WillpowerButton->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->WillpowerButton->Location = System::Drawing::Point(713, 172);
			this->WillpowerButton->Name = L"WillpowerButton";
			this->WillpowerButton->Size = System::Drawing::Size(88, 33);
			this->WillpowerButton->TabIndex = 23;
			this->WillpowerButton->Text = L"+";
			this->WillpowerButton->UseVisualStyleBackColor = true;
			this->WillpowerButton->Click += gcnew System::EventHandler(this, &LevelUp::WillpowerButton_Click);
			// 
			// focusButton
			// 
			this->focusButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->focusButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->focusButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->focusButton->Location = System::Drawing::Point(713, 217);
			this->focusButton->Name = L"focusButton";
			this->focusButton->Size = System::Drawing::Size(88, 33);
			this->focusButton->TabIndex = 24;
			this->focusButton->Text = L"+";
			this->focusButton->UseVisualStyleBackColor = true;
			this->focusButton->Click += gcnew System::EventHandler(this, &LevelUp::focusButton_Click);
			// 
			// VitalityButton
			// 
			this->VitalityButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->VitalityButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->VitalityButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->VitalityButton->Location = System::Drawing::Point(713, 88);
			this->VitalityButton->Name = L"VitalityButton";
			this->VitalityButton->Size = System::Drawing::Size(88, 33);
			this->VitalityButton->TabIndex = 25;
			this->VitalityButton->Text = L"+";
			this->VitalityButton->UseVisualStyleBackColor = true;
			this->VitalityButton->Click += gcnew System::EventHandler(this, &LevelUp::VitalityButton_Click);
			// 
			// SpellPointsLabel
			// 
			this->SpellPointsLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->SpellPointsLabel->AutoSize = true;
			this->SpellPointsLabel->Font = (gcnew System::Drawing::Font(L"Segoe Print", 18));
			this->SpellPointsLabel->Location = System::Drawing::Point(710, 292);
			this->SpellPointsLabel->Name = L"SpellPointsLabel";
			this->SpellPointsLabel->Size = System::Drawing::Size(327, 42);
			this->SpellPointsLabel->TabIndex = 26;
			this->SpellPointsLabel->Text = L"You can get 0 new spells.";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(174, 284);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(100, 50);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 27;
			this->pictureBox1->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(345, 284);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(100, 50);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox2->TabIndex = 28;
			this->pictureBox2->TabStop = false;
			// 
			// pictureBox3
			// 
			this->pictureBox3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox3.Image")));
			this->pictureBox3->Location = System::Drawing::Point(345, 490);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(100, 50);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox3->TabIndex = 29;
			this->pictureBox3->TabStop = false;
			// 
			// pictureBox4
			// 
			this->pictureBox4->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox4.Image")));
			this->pictureBox4->Location = System::Drawing::Point(345, 632);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(100, 50);
			this->pictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox4->TabIndex = 30;
			this->pictureBox4->TabStop = false;
			// 
			// pictureBox5
			// 
			this->pictureBox5->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox5.Image")));
			this->pictureBox5->Location = System::Drawing::Point(345, 764);
			this->pictureBox5->Name = L"pictureBox5";
			this->pictureBox5->Size = System::Drawing::Size(100, 50);
			this->pictureBox5->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox5->TabIndex = 31;
			this->pictureBox5->TabStop = false;
			// 
			// pictureBox6
			// 
			this->pictureBox6->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox6.Image")));
			this->pictureBox6->Location = System::Drawing::Point(164, 50);
			this->pictureBox6->Name = L"pictureBox6";
			this->pictureBox6->Size = System::Drawing::Size(100, 50);
			this->pictureBox6->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox6->TabIndex = 32;
			this->pictureBox6->TabStop = false;
			// 
			// pictureBox7
			// 
			this->pictureBox7->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox7.Image")));
			this->pictureBox7->Location = System::Drawing::Point(345, 50);
			this->pictureBox7->Name = L"pictureBox7";
			this->pictureBox7->Size = System::Drawing::Size(100, 50);
			this->pictureBox7->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox7->TabIndex = 33;
			this->pictureBox7->TabStop = false;
			// 
			// pictureBox8
			// 
			this->pictureBox8->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox8.Image")));
			this->pictureBox8->Location = System::Drawing::Point(137, 88);
			this->pictureBox8->Name = L"pictureBox8";
			this->pictureBox8->Size = System::Drawing::Size(157, 94);
			this->pictureBox8->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox8->TabIndex = 34;
			this->pictureBox8->TabStop = false;
			// 
			// pictureBox9
			// 
			this->pictureBox9->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox9.Image")));
			this->pictureBox9->Location = System::Drawing::Point(149, 327);
			this->pictureBox9->Name = L"pictureBox9";
			this->pictureBox9->Size = System::Drawing::Size(125, 84);
			this->pictureBox9->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox9->TabIndex = 35;
			this->pictureBox9->TabStop = false;
			// 
			// pictureBox10
			// 
			this->pictureBox10->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox10.Image")));
			this->pictureBox10->Location = System::Drawing::Point(149, 515);
			this->pictureBox10->Name = L"pictureBox10";
			this->pictureBox10->Size = System::Drawing::Size(145, 121);
			this->pictureBox10->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox10->TabIndex = 36;
			this->pictureBox10->TabStop = false;
			// 
			// pictureBox11
			// 
			this->pictureBox11->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox11.Image")));
			this->pictureBox11->Location = System::Drawing::Point(164, 624);
			this->pictureBox11->Name = L"pictureBox11";
			this->pictureBox11->Size = System::Drawing::Size(100, 50);
			this->pictureBox11->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox11->TabIndex = 37;
			this->pictureBox11->TabStop = false;
			// 
			// pictureBox12
			// 
			this->pictureBox12->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox12.Image")));
			this->pictureBox12->Location = System::Drawing::Point(137, 642);
			this->pictureBox12->Name = L"pictureBox12";
			this->pictureBox12->Size = System::Drawing::Size(137, 179);
			this->pictureBox12->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox12->TabIndex = 38;
			this->pictureBox12->TabStop = false;
			// 
			// SpellInfo
			// 
			this->SpellInfo->BackColor = System::Drawing::SystemColors::ControlText;
			this->SpellInfo->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->SpellInfo->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->SpellInfo->Font = (gcnew System::Drawing::Font(L"Segoe Print", 15));
			this->SpellInfo->ForeColor = System::Drawing::Color::White;
			this->SpellInfo->Location = System::Drawing::Point(713, 337);
			this->SpellInfo->Name = L"SpellInfo";
			this->SpellInfo->ReadOnly = true;
			this->SpellInfo->Size = System::Drawing::Size(480, 299);
			this->SpellInfo->TabIndex = 39;
			this->SpellInfo->Text = L"";
			// 
			// LevelUp
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlText;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(1244, 860);
			this->Controls->Add(this->SpellInfo);
			this->Controls->Add(this->pictureBox11);
			this->Controls->Add(this->pictureBox7);
			this->Controls->Add(this->pictureBox6);
			this->Controls->Add(this->pictureBox5);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->SpellPointsLabel);
			this->Controls->Add(this->VitalityButton);
			this->Controls->Add(this->focusButton);
			this->Controls->Add(this->WillpowerButton);
			this->Controls->Add(this->IntelligenceButton);
			this->Controls->Add(this->VitalityLabel);
			this->Controls->Add(this->FocusLabel);
			this->Controls->Add(this->WillpowerLabel);
			this->Controls->Add(this->IntelligenceLabel);
			this->Controls->Add(this->StatPointsLabel);
			this->Controls->Add(this->ReturnButton);
			this->Controls->Add(this->AcceptSpellButton);
			this->Controls->Add(this->Spell0);
			this->Controls->Add(this->Spell1);
			this->Controls->Add(this->Spell2);
			this->Controls->Add(this->Spell3);
			this->Controls->Add(this->Spell4);
			this->Controls->Add(this->Spell5);
			this->Controls->Add(this->Spell6);
			this->Controls->Add(this->Spell7);
			this->Controls->Add(this->Spell8);
			this->Controls->Add(this->Spell9);
			this->Controls->Add(this->Spell10);
			this->Controls->Add(this->Spell11);
			this->Controls->Add(this->Spell12);
			this->Controls->Add(this->Spell13);
			this->Controls->Add(this->Spell14);
			this->Controls->Add(this->pictureBox12);
			this->Controls->Add(this->pictureBox9);
			this->Controls->Add(this->pictureBox8);
			this->Controls->Add(this->pictureBox10);
			this->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Location = System::Drawing::Point(307, 510);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"LevelUp";
			this->Tag = L"";
			this->Text = L"Level Up";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox8))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox9))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox10))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox11))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox12))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
	private: System::Void ReturnButton_Click(System::Object^  sender, System::EventArgs^  e) {
					 this->Close();
			 }
	private: System::Void VitalityButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(hero->statPoints > 0){
					 hero->vitality+=STAT_BOOST;
					 this->VitalityLabel->Text = "Vitality: " + hero->vitality; 
					 decreaseStatPoints();
				 }}
	private: System::Void IntelligenceButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(hero->statPoints > 0){
					 hero->intelligence+=STAT_BOOST;
					 this->IntelligenceLabel->Text = "Intelligence: " + hero->intelligence;
					 decreaseStatPoints();
				 }}
	private: System::Void WillpowerButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(hero->statPoints > 0){
					 hero->willpower+=STAT_BOOST;
					 this->WillpowerLabel->Text = "Willpower: " + hero->willpower;
					 decreaseStatPoints();
				 }
			 }
	private: System::Void focusButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(hero->statPoints > 0){
					 hero->focus+=STAT_BOOST;
					 this->FocusLabel->Text = "Focus: " + hero->focus;
					 decreaseStatPoints();
				 }
			 }

	private: System::Windows::Forms::Button^ getButtonByID(int id){
				 switch (id){
					 case 0: return this->Spell0;
					 case 1: return this->Spell1;
					 case 2: return this->Spell2;
					 case 3: return this->Spell3;
					 case 4: return this->Spell4;
					 case 5: return this->Spell5;
					 case 6: return this->Spell6;
					 case 7: return this->Spell7;
					 case 8: return this->Spell8;
					 case 9: return this->Spell9;
					 case 10: return this->Spell10;
					 case 11: return this->Spell11;
				     case 12: return this->Spell12;
					 case 13: return this->Spell13;
					 case 14: return this->Spell14;
					 default: return this->Spell0;//TODO find a better default value
				 }
			 }

	private: System::Void disableGottenSpells(void){
				for(int i = 0; i < MAX_ATTACKS; i++){
					 try{
						 Button^ button = getButtonByID(hero->attack_loadout[i]->spell_id);
						 button->Enabled = false;
						 if(hero->attack_loadout[i]->spell_id == SHADOW_BALL_PLUS){
							 this->Spell0->Enabled = false;
							 (*nodes)[SHADOW_WAVE]->previousAttack = SHADOW_BALL_PLUS;
						 }else if(hero->attack_loadout[i]->spell_id == SHADOW_NEEDLE_PLUS){
							 this->Spell4->Enabled = false;
							 (*nodes)[SHADOW_SPIKES]->previousAttack = SHADOW_NEEDLE_PLUS;
						 }
					 }
					 catch(...){
						 return;//TODO make this work without exceptions?
					 }
				 }
			 }
			int findAttackIndex(int id){
			for(int i = 0; i < MAX_ATTACKS; i++){
				try{
					if(hero->attack_loadout[i]->spell_id == id){
						return i;
					}
				}
				catch(...){
					return i;//TODO make this work without exceptions?
				}
			}
			return -1;
		}
	private: System::Boolean checkPrereq(int attack){
				 if(attack == -1){
					 return true;
				 }
				 for(int i = 0; i < MAX_ATTACKS; i++){
					 try{
						 if(hero->attack_loadout[i]->spell_id == attack){
							 return true;
						 }
					 }
					 catch(...){
						 return false;//TODO make this work without exceptions?
					 }
				 }
				 return false;	
			 }

	private: System::Void Spell_Click(System::Object^  sender, System::EventArgs^  e) {
				 Button ^ s = safe_cast<Button ^>(sender);
				 AttackNode* node = (*nodes)[Convert::ToInt32(s->Tag)];
				 Boolean alreadyGotten = false;
				 if(node->attack != NULL){
					alreadyGotten = checkPrereq(node->attack->spell_id);
				 }
				 Boolean available = checkPrereq(node->previousAttack);
				 string oldStr = "Spell: " + node->name + "\n" + node->description + "\n";
				 if(available && !alreadyGotten){
					 currentNode = node;
					 oldStr += "This spell is available to pick.";
				 }else{
					 currentNode = NULL;
					 oldStr += "This spell is not available to pick.";
				 }
				 String^ str = gcnew String(oldStr.c_str());
				 this->SpellInfo->Text = str;
			 }
	private: System::Void AcceptSpellButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(currentNode != NULL && hero->spellPoints > 0){
					 int index = findLoadoutIndex();
					 if(index == -1){
						 return;
					 }
					 if(currentNode->attack->spell_id == SHADOW_BALL_PLUS){
						index = findAttackIndex(SHADOW_BALL); 
					 }
					 else if(currentNode->attack->spell_id == SHADOW_NEEDLE_PLUS){
						 index = findAttackIndex(SHADOW_NEEDLE);
					 }
					 hero->attack_loadout[index] = currentNode->attack;
					 hero->spellPoints--;
					 this->SpellInfo->Text += "\n You've just added this spell to your loadout!";
					 this->SpellPointsLabel->Text = "You can get " + hero->spellPoints + " new spells.";
					 disableGottenSpells();
				 }
			 }
	};};
