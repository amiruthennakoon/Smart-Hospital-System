#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_BEDS_PER_WARD 20

const int SPECIALTY_IDS[NUM_SPECIALTIES] = {1, 2, 3, 4};
const char SPECIALTY_NAMES[NUM_SPECIALTIES][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
const double BASE_CONSULTATION_FEES[NUM_SPECIALTIES] = {1500.00, 2500.00, 4500.00, 5000.00};
const int CONSULTATION_TIMES[NUM_SPECIALTIES] = {15, 20, 30, 30};
const int DAILY_PATIENT_CAPS[NUM_SPECIALTIES] = {30, 20, 12, 10};

const int WARD_IDS[NUM_WARDS] = {1, 2, 3, 4};
const char WARD_NAMES[NUM_WARDS][30] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU (Intensive Care Unit)"};
const double WARD_DAILY_RATES[NUM_WARDS] = {3000.00, 6000.00, 12000.00, 25000.00};
const int WARD_BED_CAPACITIES[NUM_WARDS] = {20, 10, 10, 5};

int bedOccupancy[NUM_WARDS][MAX_BEDS_PER_WARD] = {0};
int specialtyQueueCounts[NUM_SPECIALTIES] = {0};
int patientCount = 0;
char patientNames[MAX_PATIENTS][50];
int patientAges[MAX_PATIENTS];
int patientUrgencyLevels[MAX_PATIENTS];
int patientSpecialtyIDs[MAX_PATIENTS];
int patientIsAdmitted[MAX_PATIENTS];
int patientWardIDs[MAX_PATIENTS];
int patientDaysAdmitted[MAX_PATIENTS];
double patientFinalAmounts[MAX_PATIENTS];
