#include "NameGrabber.h"

NameGrabber::NameGrabber()
{
	_names.push_back("Mika");
	_names.push_back("Taylor");
	_names.push_back("Rizun");
	_names.push_back("Jesse");
	_names.push_back("Layla");
	_names.push_back("Jack");
	_names.push_back("Kyle");
	_names.push_back("Tyler");
	_names.push_back("Qualyn");
	_names.push_back("Kasey");
	_names.push_back("Adrian");
	_names.push_back("Rowan");
	_names.push_back("Nam");
	_names.push_back("Thu");
	_names.push_back("Shakes");
	_names.push_back("Michael");
	_names.push_back("Laura");
	_names.push_back("Rachael");
	_names.push_back("Sam");
	_names.push_back("Samantha");
	_names.push_back("Eric");
	_names.push_back("Erik");
	_names.push_back("John");
	_names.push_back("Jonathon");
	_names.push_back("Jennifer");
	_names.push_back("Sydnee");
	_names.push_back("Chieko");
	_names.push_back("Abel");
	_names.push_back("Aki");
	_names.push_back("Buchi");
	_names.push_back("Sho");
	_names.push_back("Kai");
	_names.push_back("Masanori");
	_names.push_back("Madoka");
	_names.push_back("Jedediah");
	_names.push_back("Aaron");
	_names.push_back("Donna");
	_names.push_back("Will");
	_names.push_back("Jordan");
	_names.push_back("Lum");
	_names.push_back("Benten");
	_names.push_back("Oyuki");
	_names.push_back("Ran");
	_names.push_back("Lux");
	_names.push_back("Orianna");
	_names.push_back("Quinn");
	_names.push_back("Zyra");
	_names.push_back("Poppy");
	_names.push_back("Caitlyn");
	_names.push_back("Kayle");
	_names.push_back("Morrigan");
	_names.push_back("Morgan");
	_names.push_back("Jade");
	_names.push_back("Alisa");
	_names.push_back("Robby");
	_names.push_back("Ayaka");
	_names.push_back("Rimika");
	_names.push_back("Mimune");
	_names.push_back("Mirei");
	_names.push_back("Honoka");
	_names.push_back("Kazari");
	_names.push_back("Jacob");
	_names.push_back("Ronald");
	_names.push_back("Jackson");
	_names.push_back("Jared");
	_names.push_back("Nick");
	_names.push_back("Lauren");
	_names.push_back("Natalie");
	_names.push_back("Beverly");
	_names.push_back("Jeff");
	_names.push_back("Bobby");
	_names.push_back("Hank");
	_names.push_back("Sejauni");
	_names.push_back("Alan");
	_names.push_back("Alex");
	_names.push_back("Danny");
	_names.push_back("Mimi");
	_names.push_back("Christy");
	_names.push_back("Yrvin");
	_names.push_back("Dana");
	_names.push_back("Miki");
	_names.push_back("Andy");
	_names.push_back("Andrew");
	_names.push_back("Binh");
	_names.push_back("Ellery");
	_names.push_back("Ryu");
	_names.push_back("Sierra");
	_names.push_back("Jackie");
	_names.push_back("Jim");
	_names.push_back("Tim");
	_names.push_back("Zac");
	_names.push_back("Victor");
	_names.push_back("Hanako");
	_names.push_back("Moka");
	_names.push_back("Brian");
	_names.push_back("Sachi");
	_names.push_back("Saori");
	_names.push_back("Kenneth");
	_names.push_back("Meysam");
	_names.push_back("Calvin");
	_names.push_back("Leum");
	_names.push_back("Tom");
	_names.push_back("Ichiro");
	_names.push_back("Vince");
	_names.push_back("Ana");
	_names.push_back("Colby");
	_names.push_back("Hilary");

	_weaponNames.push_back("Pistol");
	_weaponNames.push_back("Wand");
	_weaponNames.push_back("Broom");
	_weaponNames.push_back("Sword");
	_weaponNames.push_back("Stick");
	_weaponNames.push_back("Rock");
	_weaponNames.push_back("Axe");
	_weaponNames.push_back("Laser");
	_weaponNames.push_back("Staff");
	_weaponNames.push_back("Gopher");
	_weaponNames.push_back("Dagger");
	_weaponNames.push_back("Rifle");
	_weaponNames.push_back("Shotgun");
	_weaponNames.push_back("Spear");
	_weaponNames.push_back("Shoe");
}

NameGrabber::~NameGrabber()
{
}

std::string NameGrabber::grabName()
{
	return _names[rand() % _names.size()];
}

std::string NameGrabber::grabWeaponName()
{
	return _weaponNames[rand() % _weaponNames.size()];
}

NameGrabber gameNames;